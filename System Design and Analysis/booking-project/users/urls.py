
from django.urls import path
from . import views
from django.contrib.auth import views as auth_views
urlpatterns = [
    path('cikis/',auth_views.LogoutView.as_view(template_name = 'users/logout.html'),name = "cikis"),
    path('rezervasyon/',views.rezervasyon,name = 'rezervasyon'),
    path('gemiekle/',views.gemiEkle,name = 'gemiekle'),
    path('profil/',views.profil,name = 'profil'),

]