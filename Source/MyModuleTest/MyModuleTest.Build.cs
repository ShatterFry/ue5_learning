using UnrealBuildTool;

public class MyModuleTest : ModuleRules 
{
    public MyModuleTest(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "Blutility", "UMG"});
    }
}