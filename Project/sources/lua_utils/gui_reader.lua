File = {
	New = 
	{
		Project = true,
		Website = true,
		Blah = true,
	};
	Copy = true;
	Open = true;
}

for k,v in pairs(_G) do
    print("Global key", k, "value", v)
end