# Client TCP asincron folosind ASIO Standalone

Acest proiect demonstrează un client TCP simplu, asincron, scris în C++ folosind biblioteca ASIO standalone.
Clientul se conectează la un server web, trimite o cerere HTTP GET și afișează sau salvează răspunsul.

## Funcționalități
- Citire asincronă folosind `async_read_some` din ASIO
- Gestionarea răspunsului serverului până la închiderea conexiunii
- Funcționează cu orice server TCP (testat cu www.example.com)

## Cerințe
- C++17 sau mai nou
- Biblioteca ASIO standalone (header-only)
- CMake sau Visual Studio pentru compilare

## Utilizare
1. Descarcă și instalează ASIO standalone.
2. Compilează proiectul folosind IDE-ul preferat sau CMake.
3. Rulează executabilul; răspunsul serverului va fi afișat în consolă sau salvat într-un fișier.

## Observații
- Codul este doar pentru scopuri educaționale.
- Pentru rezultate mai clare, se recomandă testarea cu servere care oferă pagini HTML simple.
