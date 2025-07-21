The following steps should be performed when releasing a new stormpy version.

1. Update `CHANGELOG.md`:
   * To get all the commits from an author since the last tag execute:
   ```console
   git log last_tag..HEAD --author "author_name"
   ```
   * Set release month

2. Update require Storm versions:
   * Update `STORM_MIN_VERSION` in `CMakeLists.txt`

3. Check that stormpy builds without errors and all tests are successful
   * [Github Actions](https://github.com/moves-rwth/stormpy/actions) should run successfully.

4. Set new stormpy version:
   * Set new stormpy version in `lib/stormpy/_version.py`

5. Set new tag in Git (assuming that the new version is X.Y.Z and that the remote "origin" is the github repo):
   ```console
   git tag -a X.Y.Z -m "Stormpy version X.Y.Z"
   git push origin X.Y.Z
   ```
   The new tag should now be visible on [GitHub](https://github.com/moves-rwth/stormpy/tags).

6. Use the [CI](https://github.com/moves-rwth/stormpy/actions/workflows/release_docker.yml) on the tag, provide the version `X.Y.Z` as tag and the Storm base version `X.Y.Z` and automatically create the [Docker containers](https://hub.docker.com/r/movesrwth/stormpy) for the new version.

7. [Add new release](https://github.com/moves-rwth/stormpy/releases/new) in GitHub.

8. Update `stable` branch:

   ```console
   git checkout stable
   git rebase master
   git push origin stable
   ```
   Note: Rebasing might fail if `stable` is ahead of `master` (e.g. because of merge commits). In this case we can do:
    ```console
   git checkout stable
   git reset --hard master
   git push --force origin stable
   ```

9. Use the [CI](https://github.com/moves-rwth/stormpy/actions/workflows/release_docker.yml) on the `stable` branch, provide the tag 'stable' and the Storm base version `stable` automatically create the [Docker containers](https://hub.docker.com/r/movesrwth/stormpy).

10. Update Docker version for Binder by changing `binder/Dockerfile`:
    * Set new stormpy release as base
    * Update version numbers of dependencies

11. Use the [CI](https://github.com/moves-rwth/stormpy/actions/workflows/wheelpypi.yml), provide the Storm version and uncheck the 'test' option. This updates the Python package on [Pypi](https://pypi.org/project/stormpy/).
