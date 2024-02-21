""" from django.urls import path
from . import views, ListaSismos, DetalleSismo
from .views import ListaSismos, DetalleSismo
from . import views
urlpatterns = [
    path('libros/', views.ListaLibros.as_view(), name='lista-libros'),
    path('libros/<int:pk>/', views.DetalleLibro.as_view(), name='detalle-libro'),
    path('sismos/', ListaSismos.as_view(), name='lista-sismos'),
    path('sismos/<int:pk>/', DetalleSismo.as_view(), name='detalle-sismo'),
]

 """

from django.urls import path
from .views import ListaSismos, DetalleSismo  # Importación correcta desde views.py

urlpatterns = [
    path('sismos/', ListaSismos.as_view(), name='lista-sismos'),
    path('sismos/<int:pk>/', DetalleSismo.as_view(), name='detalle-sismo'),
]