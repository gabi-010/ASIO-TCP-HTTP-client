// Simple TCP Client folosind ASIO standalone
// Conecteaza la un server si citeste date HTTP asincron

#include <iostream>
#include <chrono>
#include <vector>
#include <thread>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

// Funcție asincronă pentru citirea datelor de la socket
void GrabSomeData(asio::ip::tcp::socket& socket, std::vector<char>& buffer) 
{
	socket.async_read_some(asio::buffer(buffer.data(), buffer.size()),
		[&](std::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::cout << "\n\nRead " << length << " bytes\n\n";

				// Afisează conținutul buffer-ului citit
				for (std::size_t i = 0; i < length; ++i)
					std::cout << buffer[i];

				// Continuă să citească date
				GrabSomeData(socket, buffer);
			}
			else
			{
				std::cerr << "Read error: " << ec.message() << std::endl;
			}
		}
	);
}


int main() {

	asio::error_code ec;

	// Creaze un "context", in esenta interfata specifica platformei
	asio::io_context context;

	// Implementarea unor cerinte "false" pentru a mentine context-ul activ
	asio::io_context::work idleWork(context);

	//Incepe context-ul
	std::thread thrContext([&]() { context.run(); });

	//Rezolvarrea domeniului si portului (DNS)
	asio::ip::tcp::resolver resolver(context);
	auto endpoints = resolver.resolve("www.example.com", "80", ec);

	// Verifică dacă rezolvarea DNS a eșuat
	if (ec)
	{
		std::cerr << "Resolver error: " << ec.message() << std::endl;
		return 1;
	}

	// Cream un socket-ului si conectarea la adresa IP
	asio::ip::tcp::socket socket(context);
	asio::connect(socket, endpoints, ec);

	// Verifică dacă conexiunea a eșuat
	if (ec)
	{
		std::cerr << "Connection failed: " << ec.message() << std::endl;
		return 1;
	}
	std::cout << "Connected!" << std::endl;

	// Buffer local (1 KB)
	std::vector<char> buffer(1024);
	GrabSomeData(socket, buffer);

	// Exemplu de cerere HTTP GET
	std::string sRequest =
		"GET /index.html HTTP/1.1\r\n"
		"Host: www.example.com\r\n"
		"Connection: close\r\n\r\n";

	socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);
		
	// Verifică dacă trimiterea datelor a eșuat
	if (ec)
	{
		std::cerr << "Write error: " << ec.message() << std::endl;
	}

	// Pauza pentru a lasa timp citirii datelor
	std::this_thread::sleep_for(std::chrono::seconds(10));

	// Opreste context-ul si asteapta thread-ul
	context.stop();
	if (thrContext.joinable()) 
		thrContext.join();

	return 0;
}