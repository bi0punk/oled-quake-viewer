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
                    fecha_local = fecha_local_y_lugar[:19]
                    lugar = fecha_local_y_lugar[19:].strip()
                    latitud, longitud = cols[2].split(' ')
                    profundidad = cols[3]
                    magnitud = cols[4]

                    # Limpiar la magnitud usando expresiones regulares para extraer solo el número
                    magnitud_match = re.search(r'\d+(\.\d+)?', magnitud)
                    if magnitud_match:
                        magnitud_clean = magnitud_match.group(0)
                    else:
                        magnitud_clean = "0.0"  # Asumir un valor predeterminado si no se encuentra un número

                    rows.append([fecha_local, lugar, cols[1], latitud, longitud, profundidad, magnitud_clean])

            df = pd.DataFrame(rows, columns=['Fecha Local', 'Lugar', 'Fecha UTC', 'Latitud', 'Longitud', 'Profundidad', 'Magnitud'])

            # Convertir DataFrame a lista de diccionarios
            return df.to_dict('records')
    else:
        print(f"Error en la petición web: {response.status_code}")
        return []

