from rest_framework import serializers
from .models import Quake

class DatoSerializer(serializers.ModelSerializer):
    class Meta:
        model = Quake
        fields = '__all__'
