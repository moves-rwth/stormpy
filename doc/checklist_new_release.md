The following steps should be performed before releasing a new stormpy version.

1. Update `CHANGELOG.md`
   To get all the commits from an author since the last tag execute:
   ```console
   git log last_tag..HEAD --author "author_name"
   ```

2. Update used tool versions:
   * Update `storm_min_version` in `setup.py`
   * Update `install_requires` version of pycarl in `setup.py`

3. Check that stormpy builds without errors and all tests are successful
   * [Travis](https://travis-ci.org/moves-rwth/stormpy) should run successfully

4. Set new stormpy version:
   * Set new stormpy version in `lib/stormpy/_version.py`

5. Set new tag in git
   ```console
   git tag -a new_version
   git push origin new_version
   ```
   Next we push the tag to GitHub. This step requires the GitHub repo to to be configured as a remote.
   ```console
   git remote add github https://github.com/moves-rwth/stormpy.git
   git push github new_version
   ```
   The new tag should now be visible on [GitHub](https://github.com/moves-rwth/stormpy/tags)

6. [Add new release](https://github.com/moves-rwth/stormpy/releases/new) in GitHub
