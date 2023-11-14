export OHOS_PATH=~/repo/ohos/main
change_ohos_path() {
    export OHOS_PATH=~/repo/ohos/$1
    echo "OHOS_PATH: $OHOS_PATH"
}
export OHOS_IS_SA=false
toggle_ohos_sa() {
    if [[ $OHOS_IS_SA == false ]]; then
        export OHOS_IS_SA=true
    else
        export OHOS_IS_SA=false
    fi
    echo "OHOS_IS_SA: $OHOS_IS_SA"
}