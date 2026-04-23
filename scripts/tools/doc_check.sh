#!/bin/sh

# This file can be downloaded (such as through curl) and used in CI to check
# that there are no doxygen warnings. For example, if you're using bitbucket
# pipelines then you can add the following line to your bitbucket-pipelines.yml
# file:
#   - bash <(curl -s https://github.com/aerozen/aerozen-cmake/raw/main/tools/doc_check.sh)
if [ -f aerozen-doxygen.warn ]; then
  if [ -s aerozen-doxygen.warn ]; then
    echo "Error. The following warnings were found in aerozen-doxygen.warn."
    cat aerozen-doxygen.warn
    exit 1
  else
    echo "Success. No warnings found in aerozen-doxygen.warn."
    exit 0
  fi
else
  echo "The aerozen-doxygen.warn file does not exist. Skipping doc check."
fi
