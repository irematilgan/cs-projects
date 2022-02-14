import mining_elemannet
import mining_isbulnet
import mining_kariyernet
import finalize_jsons

class JobMiner:
    def __init__(self) -> None:
        pass

    def runMiner(self,name, search_keys = None, pg_length = 1):
        if search_keys == None:
            search_keys = ['yazılım mühendisi', 'iş analisti', 'pazarlama', 'satış', 'şoför']

        if name == 'isbulnet':
            mining_isbulnet.mine(search_keys)
        if name == 'kariyernet':
            mining_kariyernet.mine(search_keys, pg_length)
        if name == 'elemannet':
            mining_elemannet.mine(search_keys)

    def formatJobs(self):
        finalize_jsons.run_formatter()

if __name__ == '__main__':
    jobminer = JobMiner()
    jobminer.runMiner('isbulnet')
    jobminer.formatJobs()
