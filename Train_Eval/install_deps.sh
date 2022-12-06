#
#  Original solution via StackOverflow:
#    http://stackoverflow.com/questions/35802939/install-only-available-packages-using-conda-install-yes-file-requirements-t
#

#
#  Install via `conda` directly.
#  This will fail to install all
#  dependencies. If one fails,
#  all dependencies will fail to install.
#
conda create -n 'embryolearning'
source activate embryolearning

conda install --yes --file requirements.txt

#
#  To go around issue above, one can
#  iterate over all lines in the
#  requirements.txt file.
#
while read requirement; do conda install --yes $requirement; done < requirements.txt

pip install opencv-python
pip install albumentations
pip install -U typing_extensions
pip install openpyxl
