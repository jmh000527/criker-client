var external0 = null;



String.prototype.format = function() {  
    if(arguments.length == 0) return this;  
    var obj = arguments[0];  
    var s = this;  
    for(var key in obj) {  
        s = s.replace(new RegExp("\\{\\{" + key + "\\}\\}", "g"), obj[key]);  
    }  
    return s;  
}  

new QWebChannel(qt.webChannelTransport,
	function(channel) {
		external0 = channel.objects.external0;
		
		
	}
);

function appendHtml0(msg){
	$("#placeholder").append(external0.msgRHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};

