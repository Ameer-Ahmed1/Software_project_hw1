#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct ClusterPoint {
  double sum;
  double mean;
  int size;
};

double find_max(double *data,int size);

void change_data_with_elm(double *data, int size, double elm);
void add_to_data(double *data, double *arr, int start, int end);
double get_euclidean_distance(double *X, struct ClusterPoint *centroid, int size);
void assignToClosestCluster(double *X, struct ClusterPoint *centroids, int size_centroids, int size_X);
int get_index_of_min(double *arr, int size);
int check_euclidean_dist_for_every_centroid(double *prevCentroids, struct ClusterPoint *centroids, int size, int size_centroid);


int main(int argc, char* argv[]) {
  int K,N,d;
  int iter;
  double *data;
  struct ClusterPoint *centroids;
  int j;
  int i;
  int c;
  double *prevCentroids; 
  double *centroid;
  double *X;
  char *str;

  
  K = atoi(argv[1]);
  N = atoi(argv[2]);
  d = atoi(argv[3]);

  if(argc < 5) {
    iter = 200;
  } else {
    iter = atoi(argv[4]);
  }

  if(K >= N || K <= 1) {
    printf("Invalid number of clusters!");
    exit(1);
  }
  if(N <= 1) {
    printf("Invalid number of points!");
    exit(1);
  }
  if(d == 0) {
    printf("Invalid dimension of point!");
    exit(1);
  }
  if(iter >= 1000 || iter <= 1) {
    printf("Invalid maximum iteration!");
    exit(1);
  }
  
  /*create a one dimensional array of size N * d of type double*/ 
  data = (double*) malloc(N*d * sizeof(double));
  i = 0; 
  j = 0;
  if (data == NULL) {
    printf("An Error Has Occurred");
    exit(1);
  }

 
  str = (char*) malloc(d*256 * sizeof(char));
  do {
    char *splitString;
    double extractedNum;
    
    c = getchar();
    if (c != '\n') {
      str[i] = c;
      i += 1;
    } else {
      
      /*for each line split it */ 
      splitString = strtok(str, ",");

      /*save the points to the array*/ 
      while (splitString != NULL)
      {
        extractedNum = strtod(splitString, NULL);
        data[j] = extractedNum;
        j += 1;
        splitString = strtok(NULL, ",");
      } 
      i = 0;
      str = (char*) malloc(d*256 * sizeof(char)); 
    }
  }while(c != EOF);

  /*init centroids as first K data points*/ 
  centroids = (struct ClusterPoint*) malloc(K*d * sizeof(struct ClusterPoint));
  j = 0;
  while (j < K*d) {
    centroids[j].sum = 0;
    centroids[j].size = 0;
    centroids[j].mean = data[j]; 
    j += 1;
  }

  /*init prevCentroids as the max value in each centroid*/ 
  prevCentroids = (double*) malloc(K*d * sizeof(double));
  centroid = (double*) malloc(d * sizeof(double));
  j = 0;

  while (j < K*d) {
    centroid[j%d] = centroids[j].mean;
    j += 1;

    if(j%d == 0) {
      double max;
      max = find_max(centroid, d);
      change_data_with_elm(centroid, d, max);
      add_to_data(prevCentroids, centroid, j - d,j);
    }
  }
    
  j = 0;
  X = (double*) malloc(d * sizeof(double));
  while(check_euclidean_dist_for_every_centroid(prevCentroids, centroids, K*d, d) == 1 && (j < iter)){
    
    int i;
    i = 0;
    /*assign every data point to its closest cluster*/ 
    while (i < N*d) {
      X[i%d] = data[i];
      i += 1;
      if(i % d == 0) {
        assignToClosestCluster(X,centroids,K*d,d);
      }
    }

    i = 0;
    /*update the centroids*/ 
    while (i < K*d) {
      prevCentroids[i] = centroids[i].mean;
      centroids[i].mean = (double)(centroids[i].sum) / (double)(centroids[i].size);
      centroids[i].sum = 0;
      centroids[i].size = 0;
      i += 1;
    }

    /*update j*/ 
    j += 1;
  }

  j = 0;

  /*show the outpot to screen*/ 
  while (j < K * d) {
    if(j%d == d-1) {
      printf("%.4f",centroids[j].mean);
    }else {
      printf("%.4f,",centroids[j].mean);
    }
    j += 1;
    if (j % d == 0) {
      printf("\n");
    }
  }
  printf("\n");

  /*free all allocated memory*/ 
  free(data);
  free(centroid);
  free(centroids);
  free(prevCentroids);
  free(X);
  free(str);

  return 0;
}


double find_max(double *data,int size) {
  double max;
  int i;

  max = data[0];
  i = 1;
  while (i < size) {
    if (data[i] > max) {
      max = data[i];
    }
    i += 1;
  }
  return max;
}

void change_data_with_elm(double *data, int size, double elm) {
  int i;

  i = 0;
  while (i < size) {
    data[i] = elm;
    i+= 1;
  }
}

void add_to_data(double *data, double *arr, int start, int end) {
  int i;
  
  i = start;
  while (i < end) {
    data[i] = arr[i - start];
    i+= 1;
  }
}

double get_euclidean_distance(double *X, struct ClusterPoint *centroid, int size) {
  double sum;
  int i;

  sum = 0;
  i = 0;
  while (i < size) {
    sum += pow(X[i] - (centroid[i].mean), 2);
    i += 1;
  }
  return sqrt(sum);
}

int get_index_of_min(double *arr, int size) {
  double min;
  int index;
  int i;

  min = arr[0];
  i = 1;  
  index = 0;
  while (i < size) {
    if(arr[i] < min) {
      min = arr[i];
      index = i;
    }
    i += 1;
  }
  return index;
}

void assignToClosestCluster(double *X, struct ClusterPoint *centroids, int size_centroids, int size_X) {
  double *argminDistValues;
  struct ClusterPoint *centroid;
  double d_X_centroid;
  int closestCluster;
  int start;
  int i;

  argminDistValues = (double*) malloc((int)(size_centroids/size_X) * sizeof(double));
  centroid = (struct ClusterPoint*) malloc(size_X * sizeof(struct ClusterPoint));  
  i = 0;
  while(i < size_centroids) {
    centroid[i%size_X].sum = centroids[i].sum;
    centroid[i%size_X].size = centroids[i].size;
    centroid[i%size_X].mean = centroids[i].mean;
    i += 1;
    if (i % size_X == 0) {
      d_X_centroid = get_euclidean_distance(X, centroid, size_X);
      argminDistValues[(int)(i/size_X)-1] = d_X_centroid;
    }
  }

  i = 0;
  closestCluster = get_index_of_min(argminDistValues, (int)(size_centroids/size_X));
  start = closestCluster*(size_X);
  while (i < size_X) {
    centroids[start + i].sum += X[i];
    centroids[start + i].size += 1;
    i += 1;
  }
  
  free(centroid);
  free(argminDistValues);
}

/*1 means its truthy 0 means its falsy*/ 
int check_euclidean_dist_for_every_centroid(double *prevCentroids, struct ClusterPoint *centroids, int size, int size_centroid) {
  int i;
  double *prevCentroid;
  struct ClusterPoint *centroid;
  double dist;

  i = 0;
  prevCentroid = (double*) malloc(size_centroid * sizeof(double));
  centroid = (struct ClusterPoint*) malloc(size_centroid * sizeof(struct ClusterPoint));
  while (i < size) {
    prevCentroid[i%size_centroid] = prevCentroids[i];
    centroid[i%size_centroid].sum = centroids[i].sum;
    centroid[i%size_centroid].size = centroids[i].size;
    centroid[i%size_centroid].mean = centroids[i].mean;

    i += 1;

    if(i%size_centroid == 0) {
      dist = get_euclidean_distance(prevCentroid, centroid, size_centroid);
      if(dist >= 0.001) {
        free(centroid);
        free(prevCentroid);
        return 1;
      }
    }
  }

  free(centroid);
  free(prevCentroid);
  return 0;
  
}