# oled-quake-viewer

Captures earthquake/seismic data via REST API and sends it to a NodeMCU ESP8266 board with an OLED screen for display. Uses Django REST backend with Celery for scheduled periodic data fetching.

## Stack

Python 3, Django REST Framework, Celery, Redis, SQLite, Arduino/NodeMCU ESP8266, OLED display

## Components

- `quakeApi/` — Django REST API with earthquake data endpoints
- Celery beat scheduler for periodic data fetching
- NodeMCU firmware for OLED display rendering

## Usage

```bash
pip install -r requirements.txt
python manage.py runserver
```

Start Celery worker:
```bash
celery -A quakeApi worker -l info
celery -A quakeApi beat -l info
```

## License

MIT
