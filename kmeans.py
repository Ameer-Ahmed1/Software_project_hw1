import sys
import math

# case iter is not provided
if (len(sys.argv)) < 6:
  try:
    K = int(sys.argv[1])
  except:
    print("Invalid number of clusters!")
    exit()
  try:
    N = int(sys.argv[2])
  except:
    print("Invalid number of points!")
    exit()
  try:
    d = int(sys.argv[3])
  except:
    print("Invalid dimension of point!")
    exit()
  iter = 200
  input_data = open(sys.argv[4], "r")

# case iter is provided
else:
  try:
    K = int(sys.argv[1])
  except:
    print("Invalid number of clusters!")
    exit()
  try:
    N = int(sys.argv[2])
  except:
    print("Invalid number of points!")
    exit()
  try:
    d = int(sys.argv[3])
  except:
    print("Invalid dimension of point!")
    exit()
  try:
    iter = int(sys.argv[4])
  except:
    print("Invalid maximum iteration!")
    exit()
  input_data = open(sys.argv[5], "r")

# inputs check:
if(K >= N or K <=1):
  print("Invalid number of clusters!")
  exit()
elif(N <= 1):
  print("Invalid number of points!")
  exit()
elif(iter <=1 or iter >= 1000):
  print("Invalid maximum iteration!")
  exit()

# add the data from the file into a list
matrix_data = []
for dataPoint in input_data:
  matrix_data.append([float(numeric_str) for numeric_str in dataPoint.replace("\n", "").split(",")])

# init centroids as first K data points
centroids = matrix_data[:K]
# init prevCentroids as the max value in each centroid
prevCentroids = [[max(centroids[i]) for j in range(d)] for i in range(K)]

# create an empty clusters list
clusters = [[] for i in range(K)]

# assign a given data point to its closest cluster
def assignToClosestCluster(X):
    argminDistValues = []

    for centroid in centroids:
      d_X_centroid = get_euclidean_dist(X,centroid)
      argminDistValues.append(d_X_centroid)

    closestCluster = argminDistValues.index(min(argminDistValues))
    clusters[closestCluster].append(X)   

# calculate the centroid of a given cluster 
def calc_centroid(cluster):
  sum = [0 for j in range(d)]
  for i in range(d):
    for X in cluster:
      sum[i] += X[i]
  centroid = [x/len(cluster) for x in sum]
  return centroid

# calculate the euclidean distance between two vectors
def get_euclidean_dist(X,Y):
  sum = 0
  for i in range(d):
    sum += math.pow(X[i]-Y[i], 2)
  return math.sqrt(sum)

# validate the euclidean distance between the previous centroids and the new ones for every cluster
def check_euclidean_dist_for_every_centroid(prevC, centroids):
  for i in range(K):
    dist = get_euclidean_dist(prevC[i], centroids[i])
    if  dist >= 0.001:
      return True
  return False

# the Algorithm
i = 0
while(check_euclidean_dist_for_every_centroid(prevCentroids, centroids) and (i < iter)):

  # assign every data point to its closest cluster
  for X in matrix_data:
    assignToClosestCluster(X)
    
  # update the centroids
  for j in range(K):
    prevCentroids[j] = centroids[j]
    new_centroid = calc_centroid(clusters[j])
    centroids[j] = new_centroid
    
  # update i
  i+=1

  # empty clusters
  for j in range(K):
    clusters[j] = []

# show the output to the screen

for i in range(K):
    print(*["%.4f" % num for num in centroids[i]], sep=",")
print("\n")