from pathlib import Path
import csv

SCRIPT_DIR = Path(__file__).resolve().parent
csv_path = SCRIPT_DIR / "test_data.csv"
new_path = SCRIPT_DIR / "new_test_data.csv"

if __name__ == "__main__":
    csv_file = open(csv_path, "r", newline='')
    write_to = open(new_path, "w", newline='')

    reader = csv.DictReader(csv_file)
    header = reader.fieldnames
    writer = csv.DictWriter(write_to, fieldnames=header)
    writer.writeheader()

    first_line = next(reader, None)
    init_time = int(first_line["t_us"])

    init_time *= -1

    line = first_line
    while line:
        write_obj = line
        line_time = int(write_obj["t_us"])

        write_obj["t_us"] = str(line_time + init_time)

        writer.writerow(write_obj)
        line = next(reader, None)

    csv_file.close()
    write_to.close()
