import json
import os

import pandas as pd
import nltk
import numpy as np
from sklearn.feature_extraction.text import CountVectorizer
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
import re
import string

def createJSON(name):
    with open(f"jobs_{name}.json","r", encoding = "utf-8") as f:
        jobs = json.load(f)
    return jobs

def cleanJobs(job_list):
    punc_list = ['\n', ',', '@', '?', '=', '$', '}', '(', '>', "'", '.', '<', ';', '\\', '{', '"', '!', '[', '|', ':', '`', ']', ')', '~', '%', '_']
    for ind in range(len(job_list)):
        if job_list[ind]['src'] == 'elemannet':
            job_list[ind]['position-info'] = []
            job_list[ind]['sector'] = ''
            job_list[ind]['general-desc'] = job_list[ind]['general-desc'].replace('\n•', '')
            
        try:
            for j, desc in enumerate(job_list[ind]['general-desc']):
                for punc in punc_list:
                    job_list[ind]['general-desc'][j] = re.sub(r'\'\w+', '', job_list[ind]['general-desc'][j])
                    job_list[ind]['general-desc'][j] = desc.replace(punc, ' ').lower()
        except Exception as e:
            pass
    
    return job_list

def analyzeJobs(jobs):
    titles_full = jobs[0]['title']
    titles_list = [jobs[0]['title']]
    desc_full = jobs[0]['general-desc']
    desc_list = [jobs[0]['general-desc']]
    criterias_full = ' '.join(list(map(''.join, jobs[0]['candidate-criterias'])))
    criterias_list = [jobs[0]['candidate-criterias']]
    positions_full = ' '.join(list(map(''.join, jobs[0]['position-info'])))
    position_list = [jobs[0]['position-info']]
    sectors_full = ''.join(list(map(''.join, jobs[0]['sector'])))
    sectors_list = [jobs[0]['sector']]
    for job in jobs[1:]:
        titles_list.append(job['title'])
        titles_full += ''.join([' ',job['title']])

        desc_list.append(job['general-desc'])
        desc_full += " " + ''.join(job['general-desc'])

        criterias_list.append(job['candidate-criterias'])
        criterias_full += ' '.join(list(map(''.join, job['candidate-criterias'])))

        position_list.append(job['position-info'])
        positions_full += ' '.join(list(map(''.join, job['position-info'])))

        sectors_list.append(job['sector'])
        sectors_full += " " +''.join(list(map(''.join, job['sector'])))

    return titles_full, titles_list, desc_full, desc_list, criterias_full, criterias_list, positions_full, position_list, sectors_full, sectors_list 
    
def createSectorDict(sectors_list):
    sector_dict = {}
    for sector in sectors_list:
        snames = sector.split(',')
        for sname in snames:
            if sname != '':
                keyName = sname.replace(' ','')
                keyName = ' '.join(re.findall('[A-ZİŞĞÜÇÖ0-9][^A-ZİŞĞÜÇÖ0-9]*',keyName))
                
                if keyName in sector_dict:
                    sector_dict[keyName] +=1
                else:
                    sector_dict[keyName] =1
    return {k: v for k, v in sorted(sector_dict.items(), key=lambda item: item[1], reverse = True)}

def createPositionDict(position_list):
    position_dict = {}
    for positions in position_list:
        for pos in positions:
            if pos != '':
                keyName = pos.replace(' ',' ')
                keyName = ''.join(re.findall('[A-ZİŞĞÜÇÖ0-9][^A-ZİŞĞÜÇÖ0-9]*',keyName))
                if keyName in "Yapıldı, Muaf, Tecilli":
                    keyName = 'Askerlik : ' + keyName
                if keyName in 'Belirtilmemiştir':
                    continue
                if keyName.isdigit() == False:
                    if keyName in position_dict:
                        position_dict[keyName] +=1
                    else:
                        position_dict[keyName] =1
    return {k: v for k, v in sorted(position_dict.items(), key=lambda item: item[1], reverse = True)}

def analyzeText(jobs_list):
    text = ''.join([job['general-desc'] for job in jobs_list if len(job['general-desc']) > 0])
    print(text)

    reText = re.sub('[^A-Za-z0-9ıöüçşğİÖÜÇŞĞ]+', ' ', text)
    stopWords = set(stopwords.words('turkish'))
    words = [word.lower() for word in reText.split() if word.lower() not in stopWords]
    tokenized_words = word_tokenize(' '.join(words))
    cv = CountVectorizer(max_features = 5000)
    transformed_words = cv.fit_transform(tokenized_words)
    return {k: v for k, v in sorted(cv.vocabulary_.items(), key=lambda item: item[1], reverse = True)}

def filterJob(keyw, jobs):
    special_jobs = []
    for job in jobs:
        if job['keyword'] == keyw:
            special_jobs.append(job)

    return special_jobs

keyw_jobs = ['yazılım mühendisi', 'iş analisti', 'pazarlama', 'satış', 'şoför']
nested_dict = {}
# kariyer_json = createJSON('kariyernet')
# eleman_json = createJSON('elemannet')
# for k in kariyer_json:
#     k['src'] = 'kariyernet'

# for k in eleman_json:
#     k['src'] = 'elemannet'
#     k['position-info'] = []
#     k['sector'] = ''
#     k['general-desc'] = k['general-desc'].replace('\n•', '')

# kariyer_json = cleanJobs(kariyer_json)
# eleman_json = cleanJobs(eleman_json)
# res_list = kariyer_json + eleman_json

# with open('jobs_final.json', 'w', encoding='utf8') as json_file:
#     json.dump(res_list, json_file, ensure_ascii=False, indent = 4)

# jobs = createJSON('final')

SITE_ROOT = os.path.realpath(os.path.dirname(__file__))
json_url = os.path.join(SITE_ROOT, "static\data", "jobs_final.json")
json_url_analyzed = os.path.join(SITE_ROOT, "", "analyzed_jobs.json")
jobs = json.load(open(json_url, encoding="utf-8"))

for keyw in keyw_jobs:
    nested_dict[keyw] = {}
    special_jobs = filterJob(keyw, jobs)
    titles_full, titles_list, desc_full, desc_list, criterias_full, criterias_list, positions_full, positions_list, sectors_full, sectors_list = analyzeJobs(special_jobs)

    print("=================== SECTORS ===================")
    sectors_json = createSectorDict(sectors_list)
    nested_dict[keyw]['sectors'] = (sectors_json)
    print(sectors_json)
    print("\n")
    print("=================== CRITERIAS ===================")
    criterias_json = createPositionDict(criterias_list)
    print(criterias_json)
    nested_dict[keyw]['criterias'] = criterias_json
    print("\n")
    print("=================== POSITIONS ===================")
    positions_json = createPositionDict(positions_list)
    nested_dict[keyw]['positions'] = (positions_json)
    print(positions_json)
    print("\n")


with open('analyzed_jobs.json', 'w', encoding='utf8') as json_file:
    json.dump(nested_dict, json_file, ensure_ascii=False, indent = 4)