
from rezervasyon.models import Gemi,Kullanici

from django import template

register = template.Library()

@register.simple_tag
def length_of_gemis(request):
    return len(Gemi.objects.all())
