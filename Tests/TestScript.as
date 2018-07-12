void GetText()
{
	toSet = "Hello, this is text from a script\nAnd this is a newline... yay\nScripts are rather nice sometimes";
}

void SetText(int &out outInt, string &out outString, int &in inInt)
{
	outInt = inInt;
	outString = "Hello, this is written using a script and a parameter that was sent to the script!";
}