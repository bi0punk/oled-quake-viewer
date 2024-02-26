import requests
from bs4 import BeautifulSoup
import datetime
import pandas as pd
import re  # Importar el módulo de expresiones regulares

def obtener_sismos():
    fecha_actual = datetime.datetime.now()
    formato_url = fecha_actual.strftime('%Y/%m/%Y%m%d')
    url = f'https://www.sismologia.cl/sismicidad/catalogo/{formato_url}.html'

    response = requests.get(url)

    if response.status_code == 200:
        soup = BeautifulSoup(response.text, 'html.parser')
        table = soup.find('table', class_='sismologia detalle')
        rows = []

        if table:
            for row in table.find_all('tr')[1:]:
                cols = [ele.text.strip() for ele in row.find_all('td')]
                if cols:
                    fecha_local_y_lugar = cols[0]
                    # Convertir fecha local a datetime
                    fecha_local = datetime.datetime.strptime(fecha_local_y_lugar[:19], '%Y-%m-%d %H:%M:%S')
                    lugar = fecha_local_y_lugar[19:].strip()
                    # Convertir latitud y longitud a float
                    latitud = float(cols[2].split(' ')[0])
                    longitud = float(cols[2].split(' ')[1])
                    # Extraer y convertir profundidad a float
                    profundidad = float(re.search(r'\d+(\.\d+)?', cols[3]).group())
                    # Extraer y convertir magnitud a float
                    magnitud = float(re.search(r'\d+(\.\d+)?', cols[4]).group())

                    rows.append({
                        'Fecha Local': fecha_local,
                        'Lugar': lugar,
                        'Fecha UTC': cols[1],  # Considera convertir esto también si es necesario
                        'Latitud': latitud,
                        'Longitud': longitud,
                        'Profundidad': profundidad,
                        'Magnitud': magnitud
                    })

    else:
        print(f"Error en la petición web: {response.status_code}")
        return []

    return rows
