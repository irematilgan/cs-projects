
from django.urls import path
from . import views
urlpatterns = [
    path('',views.home,name = "rez-home"),
    path('hakkinda/',views.about,name = "rez-about"),

]
