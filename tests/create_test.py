import random 
import os

def create_tests (folder_name, nproc_max, time_arrival_max, dt_max, deadline_max):
	
	for i in range (0, 30):

		file_name = "trace" + str (i + 1)
		#full_path = os.path.join(path, file_name)
		test = open (file_name, "w")
		
		t0 = []
		n = random.randint (2, nproc_max)
		
		for k in range (n):
			t0.append (random.randint (1, time_arrival_max));

		t0.sort()
		print(t0)

		for j in range (1, n):
			dt = random.randint (1, dt_max)
			deadline = t0[j] + dt + random.randint (1, deadline_max)
			name = "processo" + str (j)
			line = str (t0[j]) + " " + str (dt) + " " + str (deadline) + " " + name + "\n"
			test.write (line)
		test.close ()

#create_tests ("test_s", 10, 10, 10, 10)
#create_tests ("test_m", 50, 20, 20, 20)
create_tests ("test_b", 100, 30, 30, 30)