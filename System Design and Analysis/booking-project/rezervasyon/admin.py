from django.contrib import admin
from .models import Gemi,Kullanici,Yer,Rezervasyon
# Register your models here.

admin.site.register(Gemi)
admin.site.register(Kullanici)
admin.site.register(Yer)
admin.site.register(Rezervasyon)