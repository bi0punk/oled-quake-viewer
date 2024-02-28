# tasks.py
from celery import shared_task
from django.utils.dateparse import parse_datetime
from .models import Sismo
from .quake import obtener_sismos

@shared_task
def fetch_sismos_task():
    sismos_data = obtener_sismos()
    if not sismos_data:
        print("No se obtuvieron datos de sismos.")
        return

    for sismo in sismos_data:
        fecha_local = sismo['Fecha Local']
        fecha_utc = parse_datetime(sismo['Fecha UTC'])
        latitud = sismo['Latitud']
        longitud = sismo['Longitud']
        profundidad = sismo['Profundidad']
        magnitud = sismo['Magnitud']
        
        sismo_objeto, creado = Sismo.objects.get_or_create(
            fecha_utc=fecha_utc,
            latitud=latitud,
            longitud=longitud,
            defaults={
                'fecha_local': fecha_local,
                'ubicacion': sismo['Lugar'],
                'profundidad': profundidad,
                'magnitud': magnitud,
            }
        )
        
        if creado:
            print(f"Nuevo sismo agregado: {sismo_objeto}")
        else:
            print("Sismo duplicado detectado, no se agregó.")
