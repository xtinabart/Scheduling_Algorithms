/* Christina Bartsch
 * CS 441: CPU Scheduler, Project 2
 * February 16, 2025
 */
#include "scheduler.h"

int *pid, *burst, *priority, *wait, *turn; //global arrays not yet initialized

void print_usage() {
	printf("Usage: scheduler -s <1-4> [-q <quantum>] <input_file>\n");
}

void print_sched(int sch, int quant) {
	printf("Scheduler   :  ");
	if (sch == 1) {
		printf("1 FCFS\n");
		printf("Quantum     :  0\n");
	} else if (sch == 2) {
		printf("2 SJF\n");
		printf("Quantum     :  0\n");
	} else if (sch == 3) {
		printf("3 Priority\n");
		printf("Quantum     :  0\n");
	} else {
		printf("4 RR\n");
		printf("Quantum     :  %d\n", quant);
	}
}

void free_mem() {
    free(pid); free(burst); free(priority); free(wait); free(turn);
}

void print_start() {
	printf("Running...\n");
        printf("##################################################\n");
        printf("#\t#\tCPU\tPri\tW\tT\n");
}

void print_end(float avg_wait, float avg_turn) {
	printf("##################################################\n");
	printf("# Avg. Waiting Time   :  %.2f\n", avg_wait);
	printf("# Avg. Turnaround Time:  %.2f\n", avg_turn);
	printf("##################################################\n");
}

void sort_arr(int *arr, int *indices, int n) { //Bubble sort while keeping track of indices
	int temp, temp_index;
    	for (int i = 0; i < n - 1; i++) {
        	for (int j = 0; j < n - i - 1; j++) {
            		if (arr[j] > arr[j + 1]) {
                		temp = arr[j];
                		arr[j] = arr[j + 1];
                		arr[j + 1] = temp;

                		temp_index = indices[j];
                		indices[j] = indices[j + 1];
                		indices[j + 1] = temp_index;
            		}
        	}
    	}
}

void copy_arr(int *source, int *dest, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = source[i];
    }
}

void fcfs(int num) {
	int sumW = 0, sumT = 0;
	float avgW = 0.0, avgT = 0.0;
	wait[0] = 0;
	turn[0] = burst[0];
	print_start();

	for (int i = 1; i < num; i++) {
        	wait[i] = wait[i-1] + burst[i-1];
        	turn[i] = wait[i] + burst[i];
    	}

    	for (int i = 0; i < num; i++) {
        	sumW += wait[i];
        	sumT += turn[i];
    	}

	avgW = (float) sumW / num;
	avgT = (float) sumT / num;

	for (int i = 0; i < num; i++) {
		printf("#\t%d\t%d\t%d\t%d\t%d\n", pid[i], burst[i], priority[i], wait[i], turn[i]);
	}

	print_end(avgW, avgT);
	free_mem();
}

void sjf(int num) {
	print_start();
	int *og_burst = malloc(num * sizeof(int));  //Copy original burst array before sorting
    	copy_arr(burst, og_burst, num);

	int *sorted_ind = malloc(num * sizeof(int));
	for (int i = 0; i < num; i++) { //Initialize indicies array
        	sorted_ind[i] = i;
    	}

    	sort_arr(burst, sorted_ind, num); //Sort burst array from low to high and track indicies
	int sumW = 0, sumT = 0;
        float avgW = 0.0, avgT = 0.0;
        wait[sorted_ind[0]] = 0;
        turn[sorted_ind[0]] = burst[0];

	for (int i = 1; i < num; i++) {
                wait[sorted_ind[i]] = wait[sorted_ind[i-1]] + burst[i-1];
                turn[sorted_ind[i]] = wait[sorted_ind[i]] + burst[i];
        }

        for (int i = 0; i < num; i++) {
                sumW += wait[i];
                sumT += turn[i];
        }

	avgW = (float) sumW / num;
        avgT = (float) sumT / num;

        for (int i = 0; i < num; i++) {
                printf("#\t%d\t%d\t%d\t%d\t%d\n", pid[i], og_burst[i], priority[i], wait[i], turn[i]);
        }

        print_end(avgW, avgT);
        free_mem();
	free(og_burst); free(sorted_ind);
}

void pri(int num) {
	print_start();
        int *og_pri = malloc(num * sizeof(int));  //Copy original priority array before sorting
        copy_arr(priority, og_pri, num);

        int *sorted_ind = malloc(num * sizeof(int));
        for (int i = 0; i < num; i++) { //Initialize indicies array
                sorted_ind[i] = i;
        }

        sort_arr(priority, sorted_ind, num); //Sort priority array from low to high and track indicies
        int sumW = 0, sumT = 0;
        float avgW = 0.0, avgT = 0.0;
        wait[sorted_ind[0]] = 0;
        turn[sorted_ind[0]] = burst[sorted_ind[0]];

        for (int i = 1; i < num; i++) {
                wait[sorted_ind[i]] = wait[sorted_ind[i-1]] + burst[sorted_ind[i-1]];
                turn[sorted_ind[i]] = wait[sorted_ind[i]] + burst[sorted_ind[i]];
        }

        for (int i = 0; i < num; i++) {
                sumW += wait[i];
                sumT += turn[i];
        }

        avgW = (float) sumW / num;
        avgT = (float) sumT / num;

	for (int i = 0; i < num; i++) {
                printf("#\t%d\t%d\t%d\t%d\t%d\n", pid[i], burst[i], og_pri[i], wait[i], turn[i]);
        }

        print_end(avgW, avgT);
        free_mem();
        free(og_pri); free(sorted_ind);
}

void rr(int num, int q) {
	print_start();
	int *burst_leftover = malloc(num * sizeof(int));  //Copy original burst array to track remaining burst times
        copy_arr(burst, burst_leftover, num);

	int sumW = 0, sumT = 0, time_passed = 0, done = 0;
        float avgW = 0.0, avgT = 0.0;

	while (done < num) {  // Loop until all processes are done
        	for (int i = 0; i < num; i++) {
            		if (burst_leftover[i] > 0) {  // If process still has burst time leftover
                		int running_time = (burst_leftover[i] > q) ? q : burst_leftover[i];
                		time_passed += running_time;
                		burst_leftover[i] -= running_time;

                		if (burst_leftover[i] == 0) {  //If process finished, calculate wait+turn time
                    			done++;
                    			turn[i] = time_passed;
                    			wait[i] = turn[i] - burst[i];
                		}
            		}
        	} //For
    	} //While

    	for (int i = 0; i < num; i++) {
        	sumW += wait[i];
        	sumT += turn[i];
    	}

    	avgW = (float)sumW / num;
    	avgT = (float)sumT / num;

	for (int i = 0; i < num; i++) {
                printf("#\t%d\t%d\t%d\t%d\t%d\n", pid[i], burst[i], priority[i], wait[i], turn[i]);
        }

        print_end(avgW, avgT);
        free_mem();
        free(burst_leftover);
}

int main(int argc, char **argv) {
	char *input_file = NULL;
        int quantum, algor, num_process = -1;

	for (int i = 1; i < argc; i++) {
       		if (strcmp(argv[i], "-s") == 0) { //If -s found in argv
            		if (i + 1 >= argc) {
                		perror("No integer following -s.\n");
                		print_usage();
               			exit(1);
           		}
            		algor = atoi(argv[i + 1]);
            		if (algor < 1 || algor > 4) {
                		perror("Invalid scheduling type.\n");
                		print_usage();
                		exit(1);
            		}
            		i++;
        	} else if (strcmp(argv[i], "-q") == 0) { //If -q found in argv
            		if (i + 1 >= argc) {
                		perror("No integer following -q.\n");
                		print_usage();
                		exit(1);
            		}
            		quantum = atoi(argv[i + 1]);
            		if (quantum <= 0) {
                		perror("Quantum must be greater than 0.\n");
                		print_usage();
                		exit(1);
            		}
            		i++;
        	} else if (input_file == NULL) {
            		input_file = argv[i]; //Assume first non-flag arg is input file
        	} else {
            		perror("Unexpected argument.\n");
            		print_usage();
            		exit(1);
        	}
    	} //End of argument checking for loop

    	if (algor == -1) {
       		perror("No scheduling algorithm specified.\n");
        	print_usage();
        	exit(1);
    	}

    	if (algor == 4 && quantum == -1) { //Round robin algor but no quantum
        	perror("No quantum number given for round robin scheduling algorithm.\n");
        	print_usage();
        	exit(1);
    	}

    	if (input_file == NULL) {
        	perror("No input file provided.\n");
        	print_usage();
        	exit(1);
    	} //End of arg checking, start of printing and reading

	print_sched(algor, quantum);
	printf("Sch. File   : %s\n", input_file);
	printf("-------------------------------\n");

	FILE *file = fopen(input_file, "r");
    	if (!file) {
        	perror("Error opening file.\n");
        	exit(1);
    	}

    	if (fscanf(file, "%d", &num_process) != 1 || num_process <= 0) { //Read first line for number of processes
        	perror("Invalid number of processes in file.\n");
        	fclose(file);
        	exit(1);
    	}

	pid = (int *)malloc(num_process * sizeof(int)); //Allocate global array pointers
        burst = (int *)malloc(num_process * sizeof(int));
        priority = (int *)malloc(num_process * sizeof(int));
	wait = (int *)malloc(num_process * sizeof(int));
    	turn = (int *)malloc(num_process * sizeof(int));

 	if (!pid || !burst || !priority || !wait || !turn) { //Check memory allocation success
        	perror("Memory allocation failed.\n");
        	fclose(file);
        	free_mem();
        	exit(1);
    	}

	for (int i = 0; i < num_process; i++) { //Scan in process info
        	if (fscanf(file, "%d %d %d", &pid[i], &burst[i], &priority[i]) != 3) {
            		perror("Invalid process format.\n");
            		fclose(file);
            		free_mem();
            		exit(1);
        	}
	}

	printf("Arrival Order: ");
	for (int i = 0; i < num_process; i++) {
		if ((i + 1) == num_process) { //No comma on last element
			printf(" %d\n", pid[i]);
		} else {
			printf(" %d, ", pid[i]);
		}
	}

	printf("Process Information: \n");
	for (int i = 0; i < num_process; i++) {
		printf(" %d\t%d\t%d\n", pid[i], burst[i], priority[i]);
	}
	printf("-------------------------------\n"); //End of printing, start of processing

	if (algor == 1) { //Scheduling ALgorithm Calls
		fcfs(num_process);
	} else if (algor == 2) {
		sjf(num_process);
	} else if (algor == 3) {
		pri(num_process);
	} else {
		rr(num_process, quantum);
	}

    return 0;
}
