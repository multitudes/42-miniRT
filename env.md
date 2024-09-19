
### Using `conda` (if you use Anaconda/Miniconda)

1. **List your conda environments**:
   ```sh
   conda env list
   ```

create a new environment with the following command:

```sh
conda create --name myenv python=3.8
```


### Manually (if you use `venv` or `virtualenv` without `virtualenvwrapper`)

If you are not using `virtualenvwrapper` or `conda`, you can manually list your virtual environments by checking the directories where you created them. For example, if you typically create your virtual environments in a specific directory, you can list them using:

```sh
ls ~/path_to_your_virtualenvs/
```

Replace `~/path_to_your_virtualenvs/` with the actual path where you store your virtual environments.

### Using `pipenv`

If you use `pipenv` to manage your environments, you can list them with:

```sh
pipenv --venv
```

This will show the path to the virtual environment associated with the current project.

These methods should help you list and manage your virtual environments effectively.


To install the 42 formatter in a virtual environment, follow these steps:

1. **Create a virtual environment**:
   ```sh
   python3 -m venv myenv
   ```

2. **Activate the virtual environment**:
   - On macOS and Linux:
     ```sh
     source myenv/bin/activate
     ```
   - On Windows:
     ```sh
     myenv\Scripts\activate
     ```

3. **Install the 42 formatter within the virtual environment**:
   ```sh
   pip install c_formatter_42
   ```

4. **Use the formatter**:
   ```sh
   c_formatter_42 your_file.c
   ```

5. **Deactivate the virtual environment when done**:
   ```sh
   deactivate
   ```

This way, the 42 formatter and any other dependencies will be contained within the virtual environment, avoiding any impact on your global Python environment.
