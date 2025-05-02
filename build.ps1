
param(
    $action = 'init'
)

$scriptPath = split-path -parent $MyInvocation.MyCommand.Definition
# global variables
$global:workspace_dir = $scriptPath
$global:tools_dir = $global:workspace_dir + '\tools'


function Main {
    Push-Location
    Set-Location $global:workspace_dir
    
    switch ( $action.toLower() ) {
        'init'  { Start-WorkspaceInitialization }
    }
    
    Pop-Location
}

function Start-WorkspaceInitialization
{
    function WorkspaceInitialization
    {
        Import-BuildModules
    }

    Function Import-BuildModules
    {
        Push-Location
        
        Set-Location .\tools
        . .\setup\setup.ps1
        
        Pop-Location
    }
    
    WorkspaceInitialization
}

Main
