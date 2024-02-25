import os
from celery import Celery
# Asegúrate de reemplazar 'core.settings' con el nombre correcto de tu archivo de configuración
#ejemplo nombre_aplicacion.settings+---
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'core.settings')  

app = Celery('core')
app.config_from_object('django.conf:settings', namespace='CELERY')
app.autodiscover_tasks()

