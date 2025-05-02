Write-Host "Module Import" -ForegroundColor Green

Get-ChildItem -Recurse | Where-Object {$_.Name -like '*.psm1'} | ForEach-Object {
    # get module name and path
    $module = $_.BaseName
    $modulePath = $_.DirectoryName

    # import message
    $msg = "`t[Importing module {0}]" -f (Resolve-Path $modulePath -Relative)
    Write-Host $msg -ForegroundColor Green 

    # remove modules if already loaded and import module anyway
    Push-Location
    Set-Location $modulePath
    if  ((Get-Module -all | Where-Object {$_.Name -eq $module} | Measure-Object).count -gt 0)
    {
        Remove-Module $module
    }
    Import-Module $modulePath
    Pop-Location
}