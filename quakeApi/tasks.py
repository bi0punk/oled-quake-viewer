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
        latitud = float(sismo['Latitud'])
        longitud = float(sismo['Longitud'])
        profundidad = float(sismo['Profundidad'].split(' ')[0])  # Asumiendo que la profundidad viene con unidad y solo se necesita el número
        magnitud = float(sismo['Magnitud'])
        
        # Usar get_or_create para evitar duplicados basados en fecha_utc, latitud y longitud
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



