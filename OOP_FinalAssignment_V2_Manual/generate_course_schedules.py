import csv, os
import random

# Map semesters to course IDs
grouped = {
    1: [11002,11004,11063,31230,31511,41090,251961],
    2: [11006,11004,11002,11064,11121,11231,31316,31616],
    3: [11057,11064,11122,11123,11232,31350,41090,51742],
    4: [31401,31350,31402,31421,31705],
    5: [31403,31521,31442,31711,31910,31999],
    6: [11233,31017,31451,31841],
    7: [31100,31101],
    8: [31102,31103,31104]
}

# Scheduling patterns per semester
lec_days = {1:['Sunday','Monday'], 2:['Tuesday','Wednesday'], 3:['Thursday','Friday'],
            4:['Sunday','Monday'], 5:['Tuesday','Wednesday'], 6:['Thursday','Friday'],
            7:['Sunday','Monday'], 8:['Tuesday','Wednesday']}
lec_times = {sem:['08:00','10:00'] if sem<=3 else (['10:00','12:00'] if sem<=6 else ['14:00','16:00']) for sem in grouped}
tut_days = {1:['Tuesday','Thursday'],2:['Tuesday','Thursday'],3:['Wednesday','Friday'],
            4:['Wednesday','Friday'],5:['Monday','Wednesday'],6:['Monday','Wednesday'],
            7:['Tuesday','Thursday'],8:['Tuesday','Thursday']}
tut_times = {sem:['14:00','15:00'] for sem in grouped}
lab_days = {1:['Wednesday','Friday'],2:['Wednesday','Friday'],3:['Thursday','Friday'],
            4:['Tuesday','Thursday'],5:['Monday','Wednesday'],6:['Tuesday','Thursday'],
            7:['Tuesday','Thursday'],8:['Monday','Wednesday']}
lab_times = {sem:['16:00','18:00'] for sem in grouped}

# Output dir
out = 'data'
if not os.path.exists(out): os.makedirs(out)

# Read course list
detail = {}
lecturers = []
with open(os.path.join(out,'courses.csv'), encoding='utf8') as f:
    reader = csv.DictReader(f)
    for r in reader:
        cid = int(r['courseId'])
        detail[cid] = r
        lecturers.append(r['lecturer'])

# For each course generate files
for sem, ids in grouped.items():
    for cid in ids:
        row = detail.get(cid)
        if not row: continue
        lec_file = os.path.join(out, f'{cid}_lectures.csv')
        tut_file = os.path.join(out, f'{cid}_tutorials.csv')
        lab_file = os.path.join(out, f'{cid}_labs.csv')
        # choose building: physics->P
        name = row['name']
        building_lec = 'P' if 'Physics' in name else 'EM'
        building_tut = 'M'
        building_lab = 'L'
        # lectures: generate random number of alternatives per course (2-4)
        num_lec = random.randint(2,4)
        with open(lec_file,'w',newline='') as cf:
            w = csv.writer(cf)
            w.writerow(['courseId','day','startTime','duration','classroom','building','teacher','groupId'])
            for i in range(1, num_lec+1):
                d = lec_days[sem][(i-1) % len(lec_days[sem])]
                t = lec_times[sem][(i-1) % len(lec_times[sem])]
                teacher = random.choice(lecturers)
                w.writerow([cid, d, t, 2, f'Rm{i}', building_lec, teacher, f'L{i}'])
        # tutorials: random count (1-4)
        num_tut = random.randint(1,4)
        with open(tut_file,'w',newline='') as cf:
            w = csv.writer(cf)
            w.writerow(['courseId','day','startTime','duration','classroom','building','teacher','groupId'])
            for i in range(1, num_tut+1):
                d = tut_days[sem][(i-1) % len(tut_days[sem])]
                t = tut_times[sem][(i-1) % len(tut_times[sem])]
                teacher = random.choice(lecturers)
                w.writerow([cid, d, t, 1, f'Rm{10+i}', building_tut, teacher, f'T{i}'])
        # labs: random count (1-3)
        num_lab = random.randint(1,3)
        with open(lab_file,'w',newline='') as cf:
            w = csv.writer(cf)
            w.writerow(['courseId','day','startTime','duration','classroom','building','teacher','groupId'])
            for i in range(1, num_lab+1):
                d = lab_days[sem][(i-1) % len(lab_days[sem])]
                t = lab_times[sem][(i-1) % len(lab_times[sem])]
                teacher = random.choice(lecturers)
                w.writerow([cid, d, t, 3, f'Lab{i}', building_lab, teacher, f'LB{i}'])

print('Generated schedule CSVs for all courses.')
