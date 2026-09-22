$src = "E:\lab2\Assets"
$targets = @(
    "E:\lab2\Debug\Assets",
    "E:\lab2\lab2\Assets",
    "E:\lab2\lab2\Debug\Assets",
    "E:\lab2\lab2\lab2\Assets",
    "E:\lab2\lab2\lab2\Debug\Assets",
    "E:\lab2\lab2\lab2\lab2\Assets",
    "E:\lab2\lab2\lab2\lab2\Debug\Assets",
    "E:\lab2\lab2\lab2\lab2\lab2\Assets",
    "E:\lab2\lab2\lab2\lab2\lab2\Debug\Assets",
    "E:\lab2\lab2\lab2\lab2\lab2\lab2\Assets",
    "E:\lab2\lab2\lab2\lab2\lab2\lab2\Debug\Assets"
)

foreach ($tgt in $targets) {
    if (-not (Test-Path $tgt)) {
        New-Item -ItemType Directory -Force -Path $tgt | Out-Null
    }
    Copy-Item -Path "$src\*" -Destination $tgt -Force -Recurse
    $count = (Get-ChildItem $tgt).Count
    Write-Output "Synced $count items to $tgt"
}
