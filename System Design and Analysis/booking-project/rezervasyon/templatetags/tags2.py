
from rezervasyon.models import Gemi,Kullanici
from django.contrib.auth.models import User
from django import template

register = template.Library()

@register.simple_tag
def length_of_kullanicis(request):
    kullanicilar = Kullanici.objects.all()
    users = User.objects.all()
    count = 0
    for user in users:
        count += 1

    
    return count