
#=======================================================================================================================
# Copyright (C) 2021 Matvey Safroshkin, Grigory Arutyunov, Patrick Mueller
# Computer Vision Studio
# Friedrich Miescher Laboratory of the Max Planck Society
# Systems Biology of Development, University of Konstanz
# This software is distributed under the terms of the GNU General Public License
#=======================================================================================================================

import argparse
import os
import xlsxwriter


from collections import defaultdict, OrderedDict


class ExperimentDescription:

    def __init__(self, json_paths, dataset_names):

        self._description = OrderedDict()

        for json_path_pair in json_paths:

            json_path = json_path_pair[0]
            path_to_labeled_data = json_path_pair[1]

            images_count = len(os.listdir(os.path.join(path_to_labeled_data, 'images')))
            json_list = [os.path.join(path_to_labeled_data, f)
                         for f in os.listdir(path_to_labeled_data)
                         if ('json' in f) and os.path.isdir(os.path.join(path_to_labeled_data, f))]

            json_counts = [len(os.listdir(json_folder)) for json_folder in json_list]

            json_name = os.path.basename(json_path)
            json_key = os.path.splitext(json_name)[0]

            dataset_inclusion = [0] * len(dataset_names)
            for dst_idx, dataset_name in enumerate(dataset_names):
                if json_name in os.listdir(dataset_name):
                    dataset_inclusion[dst_idx] = 1

            description_entry = OrderedDict()
            for d_idx, dataset_name in enumerate(dataset_names):
                description_entry[dataset_name] = dataset_inclusion[d_idx]
            description_entry['path_to_labeled_data'] = path_to_labeled_data
            description_entry['path_to_dataset'] = []
            for dst_idx in range(len(dataset_inclusion)):
                if dataset_inclusion[dst_idx]:
                    description_entry['path_to_dataset'].append(
                        os.path.join(dataset_names[dst_idx], json_path)
                    )
            description_entry['path_to_experiment'] = os.path.normpath(
                os.path.join(path_to_labeled_data, os.pardir)
            )
            description_entry['images_count'] = images_count

            description_entry['labeler_info'] = [
                [os.path.basename(json_list[i]), json_counts[i]] for i in range(len(json_list))
            ]
            self._description[json_key] = description_entry

    @property
    def description(self):
        return self._description



def get_experiment_well_name(path_to_well):
    path_splitted = path_to_well.split(os.sep)
    experiment_folder = path_splitted[-2]
    well_folder = path_splitted[-1]
    return experiment_folder + "_" + well_folder + ".json"



def collect_experiment_description(
    labeled_data_path,
    dataset_paths):

    paths_to_wells = set()
    for root, dirs, _ in os.walk(labeled_data_path):
        if 'images' in dirs:
            paths_to_wells.add(os.path.abspath(root))

    well_json_names = set((get_experiment_well_name(ptw), ptw) for ptw in paths_to_wells)
    experiment_description = ExperimentDescription(well_json_names, dataset_paths).description

    return experiment_description


def dump_full_description_excel(description, output_dir):

    workbook = xlsxwriter.Workbook(os.path.join(output_dir, 'dataset_description.xlsx'))

    experiment_collection_dict = defaultdict(list)
    for (key, val) in description.items():
        experiment_collection_dict[val["path_to_experiment"]].append(val)
    for key in experiment_collection_dict:
        experiment_collection_dict[key] = sorted(experiment_collection_dict[key],
                                                 key=lambda x: x['path_to_labeled_data'])
        worksheet_name = os.path.basename(key)
        dump_single_description_excel(experiment_collection_dict[key], workbook, worksheet_name )

    workbook.close()


def dump_single_description_excel(description, workbook, worksheet_name):

    worksheet = workbook.add_worksheet(worksheet_name)

    output_dir = description[0]["path_to_experiment"]
    single_workbook_file_name = os.path.basename(output_dir) + ".xlsx"
    single_workbook = xlsxwriter.Workbook(os.path.join(output_dir, single_workbook_file_name))
    single_worksheet = single_workbook.add_worksheet(os.path.basename(output_dir))

    col_2_len = defaultdict(int)

    # iterating through content list
    start_column = 2
    start_row = 1

    worksheet.write(0, 0, "Drug")
    worksheet.write(0, 1, "Concentration")
    single_worksheet.write(0, 0, "Drug")
    single_worksheet.write(0, 1, "Concentration")
    col_2_len[0]= len("Drug") + 1
    col_2_len[1] = len("Concentration") + 1

    for key_indx, key in enumerate(description[0].keys()):
        if (os.path.isdir(key)):
            worksheet.write(0, start_column + key_indx, os.path.basename(key))
            single_worksheet.write(0, start_column + key_indx, os.path.basename(key))
        else:
            worksheet.write(0, start_column + key_indx, key)
            single_worksheet.write(0, start_column + key_indx, key)

        col_2_len[start_column + key_indx] = max(col_2_len[start_column + key_indx], len(str(key)))

    for row_index, ordered_dict in enumerate(description):

        for field_idx, (field_key, field_value) in enumerate(ordered_dict.items()):
            if not isinstance(field_value, list):
                worksheet.write(row_index + start_row, field_idx+start_column, field_value)
                single_worksheet.write(row_index + start_row, field_idx + start_column, field_value)

                col_2_len[field_idx + start_column] = max(col_2_len[field_idx + start_column], len(str(field_value)))
            else:
                for list_index, list_val in enumerate(field_value):
                    if isinstance(list_val, list):
                        list_str_cat =  ','.join(map(str, list_val))
                        worksheet.write(row_index + start_row, field_idx + start_column + list_index, list_str_cat)
                        single_worksheet.write(row_index + start_row, field_idx + start_column + list_index, list_str_cat)

                        col_2_len[field_idx + start_column + list_index] = max(col_2_len[field_idx + start_column + list_index], len(list_str_cat))
                    else:
                        worksheet.write(row_index + start_row, field_idx + start_column + list_index, list_val)
                        single_worksheet.write(row_index + start_row, field_idx + start_column + list_index, list_val)
                        col_2_len[field_idx + start_column + list_index] = max(col_2_len[field_idx + start_column + list_index], len(str(list_val)))

    col_offset = 16
    for col in col_2_len:
        col_len = col_2_len[col]
        worksheet.set_column(col,col, col_len + col_offset)
        single_worksheet.set_column(col, col, col_len + col_offset)

    single_workbook.close()


if (__name__=="__main__"):

    parser = argparse.ArgumentParser()
    parser.add_argument('-l', '--labeled_data_path', type=str, required=True)
    parser.add_argument('-d', '--dataset_paths', nargs="+", required=True)
    parser.add_argument('-o', '--output_dir', type=str, required=True)

    args = parser.parse_args()
    description = collect_experiment_description(
        args.labeled_data_path,
        args.dataset_paths
    )
    dump_full_description_excel(description, args.output_dir)

