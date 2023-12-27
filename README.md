# mmap-csv

mmap-csv is a lightweight C++ application designed to efficiently process large CSV files containing 'id' and 'duration' columns using the mmap system call. This application addresses scenarios where you have a list of IDs in a file named `id.csv` and multiple CSV files named in the format `input-{number}.csv` saved in a specified path.

## Input Files

### id.csv

This file contains a list of unique IDs. Example:

|    id     |
|:--------: |
| 10000022  |
| 19999998  |
| 10000008  |
|    ...    |
| 10000009  |

### input-{number}.csv

These files contain two columns, 'id' and 'duration', with entries corresponding to the given date. Example:

`input-01.csv` file:

|    id     | duration  |
|:--------: |:--------: |
| 12254257  |   9362    |
| 11058756  |   4180    |
| 11978347  |   8118    |
| 19917908  |   1675    |
|    ...    |    ...    |
| 10000009  |   7476    |

and `input-02.csv` file:

|    id     | duration  |
|:--------: |:--------: |
| 19917908  |   7350    |
| 10000009  |   7377    |
| 12630823  |   5922    |
| 10000009  |   5449    |
|    ...    |    ...    |
| 18726855  |   6869    |

## Processing and Output

The application matches the IDs in `id.csv` with those in `input-{number}.csv` files, retrieving the last duration and the longest duration for each specific ID. The results are then consolidated into an `output.csv` file.

### Example Output (output.csv)

|    id     |                              last-durations                               |                             longest-durations                             |
|:--------: |:------------------------------------------------------------------------: |:------------------------------------------------------------------------: |
| 10000008  | 8214\|5727\|9619\|4119\|4211\|4627\|3486\|4336\|2152\|4171\|3714\|6148\|  | 9619\|8214\|6148\|5727\|4627\|4336\|4211\|4171\|4119\|3714\|3486\|2152\|  |
| 10000009  |        3004\|3552\|126\|9114\|1923\|7377\|7476\|5449\|5392\|7087\|        |        9114\|7476\|7377\|7087\|5449\|5392\|3552\|3004\|1923\|126\|        |
| 10000022  |              3589\|4695\|7734\|1013\|6746\|3860\|781\|6209\|              |              7734\|6746\|6209\|4695\|3860\|3589\|1013\|781\|              |
|    ...    |                                    ...                                    |                                    ...                                    |
| 19999998  |    3938\|4828\|3877\|3123\|7316\|3716\|5258\|7953\|6889\|4241\|7402\|     |    7953\|7402\|7316\|6889\|5258\|4828\|4241\|3938\|3877\|3716\|3123\|     |
