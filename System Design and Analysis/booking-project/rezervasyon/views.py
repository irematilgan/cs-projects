from django.shortcuts import render,redirect
from django.http import HttpRequest
from .models import Gemi,Kullanici,Rezervasyon,Yer

from django.contrib.auth.models import User
from django.contrib.auth import login, authenticate
from django.contrib.auth.decorators import login_required
from django.forms import inlineformset_factory
from django.core.exceptions import PermissionDenied

"""
@login_required()
def edit_user(request,pk):
    user = User.objects.get(pk = pk)
    user_form = SignUpForm(instance = user)

    ProfileInlineFormset = inlineformset_factory(User, Kullanici, fields = ())
    formset = ProfileInlineFormset(instance = user)

    if request.user.is_authenticated() and request.user.id == user.id :
        if request.method == 'POST':
            userform = 

"""


# Create your views here.


def home(request):
    arrayKullanici = []
    arrayGemi = []
    context = {}
    kullanicilar = Kullanici.objects.all()
    gemiler = Gemi.objects.all()
    
    context = {'Users' : User.objects.all()}
    
    if Gemi.objects.all() != None:
        Gemis = Gemi.objects.all()
        context2 = {
            'Gemis' : Gemis
        }
        context.update(context2)
        
        """
        for kullanici in kullanicilar:
            arrayKullanici.append(kullanici.user.username)
            try:
                arrayGemi.append(Gemi.objects.get(pk = kullanici.user.id).isim)
            except Gemi.DoesNotExist:
                arrayGemi.append('')
            dict1 = dict(zip(arrayKullanici,arrayGemi))
            context.update(dict1)

        """
    
    
    
    
    #return HttpRequest('anasayfa.html')
    return render(request,'rezervasyon/anasayfa.html',context)

def about(request):
    context = {'title':'Hakkında'}
    return render(request,'rezervasyon/hakkinda.html',context)



