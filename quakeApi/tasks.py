from celery import shared_task
from django.utils.dateparse import parse_datetime
from .models import Sismo
from .quake import obtener_sismos

@shared_task
def fetch_sismos_task():
    sismos_data = obtener_sismos()
    for sismo in sismos_data:
        fecha_local = parse_datetime(sismo['Fecha Local'])
        fecha_utc = parse_datetime(sismo['Fecha UTC'])
        Sismo.objects.create(
            fecha_local=fecha_local,
            fecha_utc=fecha_utc,
            ubicacion=sismo['Lugar'],
            latitud=float(sismo['Latitud']),
            longitud=float(sismo['Longitud']),
            profundidad=float(sismo['Profundidad'].split(' ')[0]),  # Asumiendo que la profundidad viene con unidad y solo se necesita el número
            magnitud=float(sismo['Magnitud']),
        )
