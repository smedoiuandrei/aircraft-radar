# aircraft-radar

A real-time aircraft radar built in C, with raylib visualization and live 
ADS-B data from OpenSky Network.

## Features
- Radar window with a rotating sweep animation
- Real aircraft in a configurable geographic bounding box
- Click on an aircraft to see callsign, altitude, speed, and heading

## Stack
C11 · raylib · libcurl · cJSON · OpenSky Network API

## Build
\`\`\`bash
cmake -B build
cmake --build build
./build/radar
\`\`\`

## Architecture
See [docs/architecture.md](docs/architecture.md) for details on modules and concurrency design.
