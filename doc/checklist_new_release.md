The following steps should be performed when releasing a new stormpy version.

1. Update `CHANGELOG.md`
   * To get all the commits from an author since the last tag execute:
   ```console
   git log last_tag..HEAD --author "author_name"
   ```
   * Set release month

2. Update used tool versions:
   * Update `storm_min_version` in `setup.py`
   * Update `install_requires` version of pycarl in `setup.py`

3. Update the Storm Docker images for Github actions:
   * Set `RELEASE_IMG` and `DEBUG_IMG` in `.github/workflows/buildtest.yml`

4. Check that stormpy builds without errors and all tests are successful
   * [Github Actions](https://github.com/moves-rwth/stormpy/actions) should run successfully

5. Set new stormpy version:
   * Set new stormpy version in `lib/stormpy/_version.py`

6. Set new tag in git
   ```console
   git tag -a new_version
   git push origin new_version
   ```
   Next we push the tag to GitHub. This step requires the GitHub repo to to be configured as a remote.
   ```console
   git remote add github https://github.com/moves-rwth/stormpy.git
   git push github new_version
   ```
   The new tag should now be visible on [GitHub](https://github.com/moves-rwth/stormpy/tags).

7. [Add new release](https://github.com/moves-rwth/stormpy/releases/new) in GitHub.

8. Create [Docker containers](https://hub.docker.com/r/movesrwth/stormpy) for new version using scripts.

9. Update Docker version for Binder in by changing `binder/Dockerfile`:
    * Set new stormpy release as base
    * Update version numbers of dependencies

10. Create new python package for release on [Pypi](https://pypi.org/project/stormpy/):
   ```console
   python3 setup.py sdist
   python3 -m pip install --user --upgrade twine
   python3 -m twine upload dist/*
   ```

