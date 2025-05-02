$tterm_exe = "C:\Program Files (x86)\teraterm5\ttermpro.exe"
$ttpmacro_exe = "C:\Program Files (x86)\teraterm5\ttpmacro.exe"

# TODO: Move Fix parts to Config.json (tterm_exe, ttpmacr_exe, WBZ451_RYN241902880.ttl)

function Start-TeraTerm
{
    Push-Location

    Set-Location $global:tools_dir\tterm

    # Node 1 (WBZ451)
    # start $ttpmacro_exe $pwd\WBZ451_RYN233502665.ttl
    start $ttpmacro_exe $pwd\WBZ451H_RYN243200038.ttl

    # Node 2 (WBZ451)
    # start $ttpmacro_exe $pwd\WBZ451_RYN241902880.ttl
    # Node 2 (WBZ451HPE)
    start $ttpmacro_exe $pwd\WBZ451H_RYN243200034.ttl
    
    # Node 3 (WBZ451)
    # start $ttpmacro_exe $pwd\WBZ451_RYN232102265.ttl
    start $ttpmacro_exe $pwd\WBZ451H_RYN243200035.ttl
    
    # Node 4 (WBZ451)
    # start $ttpmacro_exe $pwd\WBZ451_RYN241902870.ttl
    start $ttpmacro_exe $pwd\WBZ451H_RYN243200037.ttl
    
    # Host Controller (SAME54 XPRO)
    # start $ttpmacro_exe $pwd\SAME54_XPRO_ATML2748041800000727.ttl
    # start $ttpmacro_exe $pwd\SAME54_XPRO_ATML2748173000000470.ttl
    start $ttpmacro_exe $pwd\SAME54_XPRO_ATML2748173000000387.ttl

    # Tool (SAME54 XPRO)
    # start $ttpmacro_exe $pwd\SAME54_XPRO_ATML2748173000000494.ttl

    # Central (WBZ351)
    start $ttpmacro_exe $pwd\WBZ351_RYN241000539.ttl

    Pop-Location
}

function Stop-TeraTerm 
{
    Push-Location

    Set-Location $global:tools_dir\tterm

    #
    # Close open TeraTerms
    #
    Get-Process ttermpro -ErrorAction SilentlyContinue | ForEach-Object {
        Stop-Process $_
    }
    Get-Process ttpmacro -ErrorAction SilentlyContinue | ForEach-Object {
        Stop-Process $_
    }

    Pop-Location
}
