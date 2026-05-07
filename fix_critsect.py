import sys
import re

def fix_critsect(file_path):
    with open(file_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # Replace mDDInterface->mCritSect with mCritSect in SexyAppBase
    # and mDDInterface->mCritSect with mApp->mCritSect in DDImage etc.
    
    if "SexyAppBase.cpp" in file_path:
        # For SexyAppBase, use its own mCritSect
        new_content = re.sub(r'mDDInterface->mCritSect', r'mCritSect', content)
    elif "DDImage.cpp" in file_path:
        # For DDImage, use its mApp->mCritSect
        # Note: I already updated DDImage to handle NULL mDDInterface, but let's be safe
        new_content = re.sub(r'mDDInterface->mCritSect', r'mApp->mCritSect', content)
    elif "D3DInterface.cpp" in file_path:
        new_content = re.sub(r'gSexyAppBase->mDDInterface->mCritSect', r'gSexyAppBase->mCritSect', content)
    else:
        new_content = content

    with open(file_path, 'w', encoding='utf-8') as f:
        f.write(new_content)

fix_critsect("SexyAppFramework/SexyAppBase.cpp")
fix_critsect("SexyAppFramework/DDImage.cpp")
fix_critsect("SexyAppFramework/D3DInterface.cpp")
print("Done")
