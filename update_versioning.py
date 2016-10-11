import os
from subprocess import call, Popen, PIPE
from sys import argv, exit    
import re


def read_command(command):
    p = Popen(command, stdout=PIPE)
    result, errors = p.communicate()
    result = result.replace("\n", "")
    return result

if len(argv) < 2:
    print("ERROR: Too few arguments.")
    print("Usage: update_versioning.py <.git dir>")
    exit(128)

git_dir = argv[1]

git_description = read_command(["git", "--git-dir", git_dir, "describe", "--always", "--tags", "--long"])
git_latest_tag = read_command(["git", "--git-dir", git_dir, "describe", "--always", "--tags", "--abbrev=0"])

git_dirty_process = Popen(["git", "diff-index", "--quiet", "HEAD", "--"])
git_dirty_process.communicate()
git_dirty = git_dirty_process.returncode
git_dirty_string = "true" if git_dirty else "false"

semver_matches = re.findall("v?(?:(\d+)\.(\d+)\.(\d+)|(\d+)\.(\d+)|(\d+))-?(.*)", git_latest_tag)
semver_major = ""
semver_minor = ""
semver_patch = ""
semver_pre_release = ""
if semver_matches > 0:
    semver_match = semver_matches[0]
    semver_pre_release = semver_match[6]
    if semver_match[0] != "":
        semver_major = semver_match[0]
        semver_minor = semver_match[1]
        semver_patch = semver_match[2]
    if semver_match[3] != "":
        semver_major = semver_match[3]
        semver_minor = semver_match[4]
    if semver_match[5] != "":
        semver_major = semver_match[5]
        
print(semver_major, semver_minor, semver_patch, semver_pre_release)

format_dict = {
    "git_description": git_description,
    "git_latest_tag": git_latest_tag,
    "git_dirty": git_dirty,
    "git_dirty_string": git_dirty_string,
    "semver_major": semver_major,
    "semver_minor": semver_minor,
    "semver_patch": semver_patch,
    "semver_pre_release": semver_pre_release,
    "cs": "{",
    "ce": "}"
}

txt_contents = """
{git_description}
{git_latest_tag}
{git_dirty}
"""
txt_contents = txt_contents.format(**format_dict)

current_contents = ""
if os.path.exists("version.txt"):
    with open("version.txt", "r") as version_file:
        current_contents = version_file.read()
if current_contents == txt_contents and False:
    print("Version contents are the same")
else:
    print("Version updated. Updating files.")
    with open("version.txt", "w") as version_file:
        version_file.write(txt_contents)
    with open("version.h", "w") as version_file:
        header_contents = """
#ifndef CUTE_VERSIONING_VERSION
#define CUTE_VERSIONING_VERSION

#include <QString>

namespace CuteVersioning {cs}
class Version {cs}
public:
    static QString latestTag() {cs} return \"{git_latest_tag}\"; {ce}
    static QString description() {cs} return \"{git_description}\"; {ce}
    static QString majorVersion() {cs} return \"{semver_major}\"; {ce}
    static QString minorVersion() {cs} return \"{semver_minor}\"; {ce}
    static QString patchVersion() {cs} return \"{semver_patch}\"; {ce}
    static QString preReleaseVersion() {cs} return \"{semver_pre_release}\"; {ce}
    static QVersionNumber versionNumber() {cs} return QVersionNumber::fromString(latestTag()); {ce}
    static bool dirty() {cs} return {git_dirty_string}; {ce}
{ce};
{ce}
#endif // CUTE_VERSIONING_VERSION
"""
        header_contents = header_contents.format(**format_dict)
        version_file.write(header_contents)
    with open("Version.qml", "w") as version_file:
        qml_contents = """
pragma Singleton
import QtQuick 2.0

QtObject {cs}
    property string major: \"{semver_major}\"
    property string minor: \"{semver_minor}\"
    property string patch: \"{semver_patch}\"
    property string preRelease: \"{semver_pre_release}\"
    property string description: \"{git_description}\"
    property string latestTag: \"{git_latest_tag}\"
    property bool dirty: {git_dirty_string}
{ce}
"""
        qml_contents = qml_contents.format(**format_dict)
        version_file.write(qml_contents)
