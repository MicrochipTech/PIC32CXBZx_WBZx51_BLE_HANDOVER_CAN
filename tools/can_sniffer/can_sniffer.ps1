function Start-CanSniffer ()
{
    Push-Location

    Set-Location $global:tools_dir\can_sniffer
    Write-Host 'Start-CanSniffer'
    & py -3 $pwd/src/can_sniffer.py
    
    Pop-Location
}
    