<p align="center"><img src="https://socialify.git.ci/bi0punk/oled-quake-viewer/image?language=1&amp;name=1&amp;owner=1&amp;stargazers=1&amp;theme=Light" alt="project-image"></p>

<p id="description">Django Rest python project that captures and sends data to the NodeMCU ESP8266 development board to view via OLED screen</p>

  
  
<h2>💻 Built with</h2>

Technologies used in the project:

*   Python
*   Arduino
*   NodeMCU
*   Flask Rest Framework
*   Sqlite
*   Celery
*   Redis Server

- celerybeat-schedule.db
- core
  - asgi.py
  - celery.py
  - __init__.py
  - settings.py
  - urls.py
  - wsgi.py
- db.sqlite3
- manage.py
- oled_screen_data
  - oled_screen_data.ino
- quakeApi
  - admin.py
  - apps.py
  - __init__.py
  - migrations
    - 0001_initial.py
    - 0002_sismo.py
    - __init__.py
  - models.py
  - quake.py
  - serializers.py
  - tasks.py
  - templates
    - quakeApi
      - index.html
  - tests.py
  - urls.py
  - views.py
- README.md
- requirements.txt
- sismos_respaldo.csv
