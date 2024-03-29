' https://superuser.com/questions/59465/is-it-possible-to-download-using-the-windows-command-line#answer-59489
If WScript.Arguments.Length < 2 Then
	Wscript.Echo "Usage: " + Wscript.ScriptName + " <url> <local file>"
	WScript.Quit(1)
Else
	strFileURL = WScript.Arguments.Item(0)
	strHDLocation = WScript.Arguments.Item(1)

	Set objXMLHTTP = CreateObject("MSXML2.XMLHTTP.6.0")

	objXMLHTTP.open "GET", strFileURL, false
	objXMLHTTP.send()

	If objXMLHTTP.Status = 200 Then
		Set objADOStream = CreateObject("ADODB.Stream")
		objADOStream.Open
		objADOStream.Type = 1 'adTypeBinary

		objADOStream.Write objXMLHTTP.ResponseBody
		objADOStream.Position = 0    'Set the stream position to the start

		Set objFSO = Createobject("Scripting.FileSystemObject")
		If objFSO.Fileexists(strHDLocation) Then objFSO.DeleteFile strHDLocation
		Set objFSO = Nothing

		objADOStream.SaveToFile strHDLocation
		objADOStream.Close
		Set objADOStream = Nothing
	End if

	Set objXMLHTTP = Nothing
End If
