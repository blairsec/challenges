# Problems

This repository hosts AngstromCTF problems, problem files, scripts, and other utilities necessary to deploy and run the competition.

Problems are organized into branches by year, so make sure you're working from the correct branch. To access problems from 2017, for example, run the following command from this repository's root directory:

```bash
git checkout 2017
```

## Contributing

Problems are organized by category, and should be contained in subdirectories with concise names that do not include spaces. When delimiting words, underscores are strongly encouraged as opposed to camel case.

### Problem Metadata

Each problem directory must contain a file named `problem.yml`, which should contain problem metadata such including the problem's full name, text, flag, and files. An example `problem.yml` should be as follows:
 
```yaml
title: My Problem
author: Me
value: 100
text: |
  The body text of the problem. Can be formatted however is desired by the
  problem writer within YAML specifications. Text will be formatted with a 
  markdown converter prior to deployment. URL: {{ my_file1.txt }}.
hint: A hint about the problem.
flag: actf{my_problem_flag}
files: 
  - my_file1.txt
  - my_file2.txt
enabled: true
```

The `files` field is optional, but if it is set to a list of file names, the corresponding files, located relative to the path of the `problem.yml` file, will be copied into the static assets of the problem server. The `enabled` field is also optional but defaults to `true`. If set to false, the problem will not be deployed on the server.

In addition, note that prior to conversion to markdown, the `text` field is searched for URL tags pointing to `files`. A macro `{{ my_file1.txt }}` will be replaced with the raw URL to the where the file is stored on the deployed problem server, which might be something like:

```
https://angstromctf.com/problems/<category>/<name>/my_file1.txt
```

### Problem Build Scripts

In addition to metadata, problems may define a build script. A file named `build.sh` in your problem directory will be executed when the problem is deployed. Use this feature to build executables or compile dependencies.

## Problem Deployment

Problem servers can be automagically deployed in production by specifying a `deploy` field inside of the problem metadata. The field should contain a subtree containing the deployment type and deployment-specific parameters:

```yaml
deploy:
  type: <type>
  ...
```

### Docker

One of the most common deployment configurations is Docker, which is a containerization service that we can use to run web servers for problems. In order to deploy with Docker, add a metadata entry similar to the following:

```yaml
deploy:
  type: docker
  ports: 
    80: 5000
```

The `ports` field should consist of a list of `from: to` pairs that specify how ports inside the container are mapped to the outside.

For docker, you must also have a Dockerfile inside of your problem directory. This actually specifies the configuration of the server. Sample Dockerfiles for various configurations will be added at a later point, so for now, consult the [Docker documentation](https://docs.docker.com/engine/reference/builder/).
