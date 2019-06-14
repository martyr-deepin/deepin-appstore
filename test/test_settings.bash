declare -A keys

keys=( \
    ["AutoInstall"]="" \
    ["ThemeName"]="" \
    ["WindowState"]="" \
    ["AllowShowPackageName"]="" \
    ["MetadataServer"]="" \
    ["OperationServerMap"]="" \
    ["DefaultRegion"]="" \
    ["AllowSwitchRegion"]="" \
    ["SupportSignIn"]="" \
    ["UpyunBannerVisible"]="" \
    ["SupportAot"]="" \
)


for key in ${!keys[@]}
do
    echo "----- $key"
    dbus-send --print-reply --session --dest=com.deepin.AppStore.Daemon \
    /com/deepin/AppStore/Settings \
    com.deepin.AppStore.Settings.GetSettings \
    string:"$key"
    echo "----------"
done



