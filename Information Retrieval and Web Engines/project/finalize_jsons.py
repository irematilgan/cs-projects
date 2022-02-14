import json
import re

def cleanJobs(job_list):
    punc_list = ['\n', ',', '@', '?', '=', '$', '}', '(', '>', "'", '.', '<', ';', '\\', '{', '"', '!', '[', '|', ':', '`', ']', ')', '~', '%', '_']
    for ind in range(len(job_list)):
        if job_list[ind]['src'] == 'elemannet' or job_list[ind]['src'] == 'isbulnet':
            job_list[ind]['position-info'] = []
            job_list[ind]['sector'] = ''
            job_list[ind]['general-desc'] = job_list[ind]['general-desc'].replace('\n•', '')
        if job_list[ind]['src'] == 'isbulnet':
            job_list[ind]['candidate-criterias'] = []

        try:
            for j, desc in enumerate(job_list[ind]['general-desc']):
                for punc in punc_list:
                    job_list[ind]['general-desc'][j] = re.sub(r'\'\w+', '', job_list[ind]['general-desc'][j])
                    job_list[ind]['general-desc'][j] = desc.replace(punc, ' ').lower()
        except Exception as e:
            pass
    
    return job_list

def createJSON(name):
    with open(f"jobs_{name}.json","r", encoding = "utf-8") as f:
        jobs = json.load(f)
    return jobs

def run_formatter():
    print("============== STARTING FORMATTER ==============")
    kariyer_json = createJSON('kariyernet')
    eleman_json = createJSON('elemannet')
    isbul_json = createJSON('isbulnet')

    kariyer_json = cleanJobs(kariyer_json)
    eleman_json = cleanJobs(eleman_json)
    isbul_json = cleanJobs(isbul_json)
    res_list = kariyer_json + eleman_json + isbul_json

    with open('jobs_final.json', 'w', encoding='utf8') as json_file:
        json.dump(res_list, json_file, ensure_ascii=False, indent = 4)

    print("============== FORMATTING COMPLETED ==============")