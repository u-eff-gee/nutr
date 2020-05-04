import os
os.chdir('@PROJECT_BINARY_DIR@/src/fundamentals/primary_generator/angular_correlation_source/python/')
from angular_correlation import angular_correlation

print(angular_correlation(0., 0.))