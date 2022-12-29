
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

from .ExperimentCollector import ExperimentCollector
import json
import csv
import os
from csv import  reader
from matplotlib import pyplot as plt
import seaborn as sns
from sklearn.metrics import confusion_matrix, classification_report , f1_score, accuracy_score
import shutil

def plot_confusion_matrices(output_folder,
                            cfg_filename,
                            disable_numbers,
                            name,
                            experiment_list,
                            labeler_names,
                            algorithm_name,
                            filters,
                            class_names,
                            with_severities,
                            classes_with_severities,
                            severities,
                            fig_size):

    EC = ExperimentCollector(experiment_list, labeler_names, algorithm_name,filters, True)
    EC.collect_experimetns()
    EC.collect_jsonFiles()
    EC.collect_detections(with_severity=with_severities)
    class_names = EC.map_class_list(class_names,with_severities,classes_with_severities,severities)

    # Filter only fro selected classes
    #print("class_names : ", class_names)
    #print("EC.human_labels before:", len(EC.human_labels))
    EC.human_labels, EC.algotightm_labels = filter_labels(EC.human_labels, EC.algotightm_labels, class_names)
    #print("EC.human_labels after :", len(EC.human_labels))
    
    #measure confusion matrix
    cm_normal = confusion_matrix(EC.human_labels, EC.algotightm_labels, labels = class_names)

    _, newMatrix = EC.plot_confusion_matrix(cm_normal, class_names)
    fig_size= tuple(fig_size)
    plt.figure(figsize=fig_size)  # Push new figure on stack
    ax = sns.heatmap(newMatrix, linewidth=0.5,
                     xticklabels=class_names,
                     yticklabels=class_names,                     
                     vmin=0, vmax=1,
                     cbar=0,
                     annot=not disable_numbers,
                     annot_kws={"size": 18},
                     fmt="0.2f",
                     cmap = "viridis" )
    print("name : ", os.path.splitext(os.path.basename(cfg_filename))[0])
    cfg_filename = os.path.splitext(os.path.basename(cfg_filename))[0]
#    if (not name):
#        cfg_filename = os.path.splitext(os.path.basename(cfg_filename))[0]
#    else:
#        cfg_filename = name +"_" + os.path.splitext(os.path.basename(cfg_filename))[0]

    plt.savefig(os.path.join (output_folder, cfg_filename + '_cm.svg'))  # Save that figure
    plt.figure(figsize=fig_size)
    path_to_confusion_matrix_csv = os.path.join(output_folder, cfg_filename + "_cm.csv")
    path_to_confusion_bins = os.path.join(output_folder, cfg_filename + "_bins.csv")
    csv_confusion_matrix_list = EC.get_csv_list(newMatrix, class_names)
    with open(path_to_confusion_matrix_csv, "w", newline='') as fp:
        writer = csv.writer(fp, delimiter=",")
        writer.writerows(csv_confusion_matrix_list)

    csv_confusion_bin_list = EC.get_csv_list(cm_normal, class_names)
    with open(path_to_confusion_bins, "w", newline='') as fp:
        writer = csv.writer(fp, delimiter=",")
        writer.writerows(csv_confusion_bin_list)
        
    print("CM : ", cm_normal)
    print("CM sum: ", sum(sum(cm_normal)))
        
    #-------------------------------------------------------------
    #f1_score
    f1_score_ = f1_score(EC.human_labels, EC.algotightm_labels, labels=class_names, average=None)
    #print("f1_score : ", f1_score_)

    path_to_f1_score_csv = os.path.join(output_folder, cfg_filename + "_f1_score.csv")
    with open(path_to_f1_score_csv, "w", newline='') as fp:
        writer = csv.writer(fp, delimiter=",")
        writer.writerow(class_names)
        writer.writerow(f1_score_)

    # -------------------------------------------------------------
    #classification_report
    report = classification_report (EC.human_labels, EC.algotightm_labels, labels=class_names)
    path_to_report_csv = os.path.join(output_folder, cfg_filename + "_report.txt")
    with open(path_to_report_csv, "w", newline='') as fp:
        fp.write(report)
        
        
    # -------------------------------------------------------------
    #accuracy_score
    accuracyscore = accuracy_score(EC.human_labels, EC.algotightm_labels, normalize=True)
    print("accuracy_score: ", accuracyscore)
    accuracyscore1 = accuracy_score(EC.human_labels, EC.algotightm_labels, normalize=False)
    print("accuracy_score: ", accuracyscore1)  
    path_to_accuracy_csv = os.path.join(output_folder, cfg_filename + "_accuracy.txt")
    with open(path_to_accuracy_csv, "w", newline='') as fp:
        writer = csv.writer(fp, delimiter=",")
        writer.writerow([accuracyscore])
    
    print("TP recalculated : ", accuracyscore*sum(sum(cm_normal)))    
        
def filter_labels(human_labels, algorithm_labels, class_names):
    new_human_labels =[]
    new_algorithm_labels=[]
    for i in range(0, len(human_labels)):
        #if ((human_labels[i] != "CUT") and (algorithm_labels[i] != "CUT") and (human_labels[i] != "NODAL_25") and (algorithm_labels[i] != "NODAL_25") and (human_labels[i] != "BMP_25") and (algorithm_labels[i] != "BMP_25")):
        if ((human_labels[i] in class_names) and (algorithm_labels[i] in class_names)):
        #if (algorithm_labels[i] in class_names):
            new_human_labels.append(human_labels[i])
            new_algorithm_labels.append(algorithm_labels[i])

    return new_human_labels, new_algorithm_labels 
           
