# oled-quake-viewer

Captures earthquake/seismic data via REST API and displays it on a NodeMCU ESP8266 with an OLED screen. Uses Django REST backend with Celery for periodic data fetching and scheduling.

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Python](https://img.shields.io/badge/Python-3.11+-blue)](https://python.org)
[![CI](https://github.com/tu-usuario/oled-quake-viewer/actions/workflows/ci.yml/badge.svg)](https://github.com/tu-usuario/oled-quake-viewer/actions/workflows/ci.yml)

## Tabla de Contenidos

- [Características](#características)
- [Stack](#stack)
- [Arquitectura](#arquitectura)
- [Requisitos](#requisitos)
- [Instalación](#instalación)
- [Uso](#uso)
- [Tests](#tests)
- [Configuración](#configuración)
- [CI](#ci)
- [Datos](#datos)
- [Limitaciones / Roadmap](#limitaciones--roadmap)
- [Licencia](#licencia)

## Características

- Consulta periódica de datos sísmicos desde API REST externa
- Visualización en display OLED via NodeMCU ESP8266
- Backend Django REST Framework con endpoints de sismos
- Programación de tareas con Celery Beat + Redis
- Almacenamiento local en SQLite con respaldo CSV

## Stack

- Python 3.11+, Django 4.2+, Django REST Framework
- Celery 5.3 + Redis (broker/backend)
- SQLite, pandas, requests, BeautifulSoup
- NodeMCU ESP8266, display OLED

## Arquitectura

```
oled-quake-viewer/
├── core/
│   ├── celery.py
│   ├── settings.py
│   └── urls.py
├── quakeApi/
│   ├── models.py
│   ├── serializers.py
│   ├── tasks.py          # Tareas Celery
│   ├── views.py
│   ├── urls.py
│   └── templates/
├── tests/
├── manage.py
├── requirements.txt
├── pyproject.toml
└── .env.example
```

## Requisitos

- Python 3.11+
- Redis server (para Celery)
- NodeMCU ESP8266 con display OLED (opcional, solo visualización física)

## Instalación

```bash
git clone https://github.com/tu-usuario/oled-quake-viewer.git
cd oled-quake-viewer
python -m venv .venv && source .venv/bin/activate
pip install -r requirements.txt
python manage.py migrate
```

## Uso

Iniciar servidor Django y workers Celery:

```bash
# Servidor de desarrollo
python manage.py runserver

# Worker Celery (terminal 2)
celery -A quakeApi worker -l info

# Beat scheduler (terminal 3)
celery -A quakeApi beat -l info
```

## Tests

```bash
pip install pytest ruff
pytest -q
ruff check .
```

## Configuración

Variables de entorno (ver `.env.example`):

| Variable       | Descripción                     |
|----------------|---------------------------------|
| `DATABASE_URL` | URL de base de datos (SQLite)   |
| `REDIS_URL`    | URL de Redis para Celery        |
| `API_KEY`      | API key del servicio sísmico    |

## CI

GitHub Actions ejecuta lint (ruff) + pytest en cada push y PR usando Python 3.11 con uv.

## Datos

Los datos sísmicos se almacenan en SQLite (`db.sqlite3`) con respaldo periódico en `sismos_respaldo.csv`. El modelo incluye campos para magnitud, ubicación, profundidad y timestamp.

## Limitaciones / Roadmap

- [ ] Soporte para múltiples displays OLED
- [ ] Dashboard web con gráficos históricos
- [ ] Alertas en tiempo real via WebSocket
- [ ] Dockerización completa del stack

## Licencia

MIT
