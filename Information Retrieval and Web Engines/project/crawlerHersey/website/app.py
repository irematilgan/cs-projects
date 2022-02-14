from flask import Flask, request, render_template, abort, url_for, json, jsonify
import os
import json
import html
import mining
import analyze_jobs

app = Flask(__name__)


@app.route('/')
def hello():
    return render_template("index.html")


def filterByWebsite(jsonList, site_name, job_name,ilan_counter):
    newList = []
    for element in jsonList:
        # print(element["src"])
        if "src" in element:
            if element["src"] == site_name and element["keyword"] == job_name:
                newList.append(element)

    prettyText = ""
    for element in newList:
        ilan_counter+=1
        prettyText+="<p>"
        prettyText += "<strong>Başlık: </strong>" + element['title'] + "\n"
        prettyText += "<strong>Kriterler: </strong>" + "\n"
        for item in element['candidate-criterias']:
            prettyText += item + "\n"

        prettyText += "<strong>Pozisyon Bilgileri: </strong>" + "\n"
        for item in element['position-info']:
            prettyText += item + "\n"

        prettyText += "<strong>Sektör: </strong>" + element['sector'] + "\n"
        prettyText+="<strong>URL: </strong>" + "<a href="+element['url']+">"+element['url']+"</a>"+"\n"
        prettyText += "<strong>Açıklama: </strong>" + "\n"
        for item in element['general-desc']:
            prettyText += item
            # prettyText+= item + "\n"
        # prettyText += "<hr>"
        prettyText += "</p><br>"
    return prettyText,ilan_counter


@app.route('/response', methods=['POST'])
def response():
    jobName = request.form.get("jobs")

    SITE_ROOT = os.path.realpath(os.path.dirname(__file__))
    json_url = os.path.join(SITE_ROOT, "static\data", "jobs_final.json")
    json_url_analyzed = os.path.join(SITE_ROOT, "", "analyzed_jobs.json")
    # print(json_url)
    data = json.load(open(json_url, encoding="utf-8"))
    data_analyzed = json.load(open(json_url_analyzed, encoding="utf-8"))
    # print(data)
    # print(data_analyzed[jobName]["sectors"])
    sektor_listesi = []
    sektor_values = []

    for item in data_analyzed[jobName]["sectors"].keys():
        sektor_listesi.append(item)
    for item in data_analyzed[jobName]["sectors"].values():
        sektor_values.append(item)
    if len(sektor_listesi)>10:
        sektor_listesi = sektor_listesi[0:10]
        sektor_values = sektor_values[0:10]

    # print(sektor_listesi)
    # print(sektor_values)

    KRITER_SAYISI = 5
    criter_listesi = []
    criter_values = []
    counter = 0
    for item in data_analyzed[jobName]["criterias"].keys():
        counter += 1
        criter_listesi.append(item)
        if counter == KRITER_SAYISI:
            break
    counter = 0
    for item in data_analyzed[jobName]["criterias"].values():
        counter += 1
        criter_values.append(item)
        if counter == KRITER_SAYISI:
            break
    critersafelist=""
    i=0
    while(i<KRITER_SAYISI):
        critersafelist+="<li>"+criter_listesi[i]+" : <strong>"+str(criter_values[i])+"</strong></li>"
        i+=1

    POZISYON_SAYISI = 5
    position_listesi = []
    position_values = []
    counter = 0
    for item in data_analyzed[jobName]["positions"].keys():
        counter += 1
        position_listesi.append(item)
        if counter == POZISYON_SAYISI:
            break
    counter = 0
    for item in data_analyzed[jobName]["positions"].values():
        counter += 1
        position_values.append(item)
        if counter == POZISYON_SAYISI:
            break
    pozisyon_listesi = ""
    i = 0
    while (i < POZISYON_SAYISI):
        pozisyon_listesi += "<li>" + position_listesi[i] + " : <strong>" + str(position_values[i]) + "</strong></li>"
        i += 1
    kariyerNet=""
    elemanNet = ""
    isbulNet = ""
    ilan_counter=0
    if request.form.get("kariyernet")=="on":
        kariyerNet,ilan_counter = filterByWebsite(data, "kariyernet", jobName,ilan_counter)
    if request.form.get("elemannet")=="on":
        elemanNet,ilan_counter = filterByWebsite(data, "elemannet", jobName,ilan_counter)
    if request.form.get("isbul")=="on":
        isbulNet,ilan_counter = filterByWebsite(data, "isbulnet", jobName,ilan_counter)




    text_results = jobName.capitalize() +" arama sonuçları ("+str(ilan_counter)+" adet):"
    # niceText = json.dumps(data, ensure_ascii=False, indent=4, sort_keys=True)
    # print(niceText)
    return render_template("index.html",positionlist=pozisyon_listesi,positiontitle="Pozisyon Bilgisi",criterastitle="Kriterler", criterlist=critersafelist,
                           sektorlist=sektor_listesi, sektorvalues=sektor_values, selectedjob=text_results,
                           kariyerList=kariyerNet,
                           isbulList=elemanNet, elemanList=isbulNet)



@app.route('/mine', methods=['POST'])
def mine():
    miner = mining.JobMiner()
    print(request.form.get("isbul2"))
    if request.form.get("kariyernet")=="on":
        print("kariyer mining")
        miner.runMiner('kariyernet')
    if request.form.get("elemannet2")=="on":
        print("eleman mining")
        miner.runMiner('elemannet')
    if request.form.get("isbul2")=="on":
        print("isbul mining")
        miner.runMiner('isbulnet')

    miner.formatJobs()
    return render_template("index.html")
if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000)
