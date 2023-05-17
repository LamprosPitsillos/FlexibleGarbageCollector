import os


def get_csv_file_paths(directory):
    csv_paths = {}
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(".csv"):
                folder_name = os.path.basename(root)
                file_path = os.path.join(root, file)
                parts = file.split("_")
                label = " ".join(parts[2:]).split(".")[0]
                csv_paths.setdefault(folder_name, {}).update({label: file_path})
    return csv_paths


directory = "../Data/Logs"
csv_paths = get_csv_file_paths(directory)
__import__('pprint').pprint(csv_paths)
print(csv_paths["simple_test"]["Tracing RDFS"])
