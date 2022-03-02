# PIR – nano33ble

<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/PIR-IDS/nano33ble">
    <img src="https://avatars.githubusercontent.com/u/99486891" alt="Logo" width="130">
  </a>

  <p align="center">
    IDS: Code for the nano33ble board
    <br />
    <a href="https://github.com/PIR-IDS/nano33ble/releases"><strong>See Releases »</strong></a>
    <br />
    <br />
    <a href="#">Research Paper</a>
    ·
    <a href="https://github.com/PIR-IDS/nano33ble/actions/workflows/test.yml">Test Results</a>
    ·
    <a href="https://github.com/PIR-IDS/nano33ble/projects">Show Project</a>
  </p>

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage">Usage</a>
      <ul>
        <li><a href="#execution">Execution</a></li>
        <li><a href="#generation">Generation</a></li>
        <li><a href="#tests">Tests</a></li>
        <li><a href="#documentation">Documentation</a></li>
      </ul>
    <li><a href="#tree-structure">Tree Structure</a></li>
    <li><a href="#credits">Credits</a></li>
    <li><a href="#contact">Contact</a></li>

  </ol>
</details>

***

<!-- ABOUT THE PROJECT -->
## About The Project

This code will be used in order to detect anomalies and to inform our IDS about it.

### Built With
* [C++](https://isocpp.org/)
* [Arduino Framework](https://docs.platformio.org/en/stable/frameworks/arduino.html)

<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

1. Install the latest version of Python 3 with `pip` (included): https://www.python.org/downloads/
2. Install the latest version of PlatformIO: https://docs.platformio.org/en/latest//core/installation.html#installation-methods
    ```sh
    pip install -U platformio
    ```

### Installation

1. Clone the project
   ```sh
   git clone https://github.com/PIR-IDS/nano33ble.git
   ```
2. Install the dependencies by typing the following command while being in the project root:
   ```sh
   pio lib install
   ```

#### Using an IDE

If you want to use a specific IDE with this project, you can list the compatible ones by checking the `--ide` line with:
```sh
pio init -h
```

Choose your IDE with:
```sh
pio init --ide <ide>
```

> :warning: **DO NOT COMMIT THE NEWLY CREATED FILES AFTER THE IDE INIT COMMAND.**
> 
> You can add their name/location in the `.git/info/exclude` file to exclude them from the VCS without affecting the `.gitignore` file. You will also have to add the build directories created by your IDE in order to keep your artifacts outside of the project repository.


<!-- USAGE EXAMPLES -->
## Usage

### Execution

Use the following command in the project root to build and upload the firmware in the default `debug` mode (**with the board plugged**):
  ```sh
  pio run -t upload
  ```

By adding the `-e release` option you will be in `release` mode.

### Generation

You can also build the firmware in the default `debug` mode without uploading:
  ```sh
  pio run
  ```

Again, by adding the `-e release` option you will be in `release` mode.

### Tests

_TODO_

### Documentation

_TODO_

***

<!-- TREE STRUCTURE -->
## Tree Structure
<details>

_TODO_

</details>

<!-- CREDITS -->
## Credits

Romain Monier [ [GitHub](https://github.com/rmonier) ] – Co-developer
<br>
Morgan Pelloux [ [GitHub](https://github.com/MonsieurSinge) ] – Co-developer
<br>
David Violes [ [GitHub](https://github.com/ViolesD) ] – Co-developer
<br>
Malik Sedira [ [GitHub](https://github.com/sediramalik) ] – Co-developer
<br>
Quentin Douarre [ [GitHub](https://github.com/Quintus618) ] – Co-developer
<br>
Noé Chauveau [ [GitHub](https://github.com/Noecv) ] – Co-developer
<br>
Pierre Favary [ [GitHub](https://github.com/pdf-0) ] – Co-developer

<!-- CONTACT -->
## Contact

Project Link : [https://github.com/PIR-IDS/nano33ble](https://github.com/PIR-IDS/nano33ble)

Organization Link : [https://github.com/PIR-IDS](https://github.com/PIR-IDS)