<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="icon" href="${root}/favicon.ico" type="image/x-icon"/>
<link rel="shortcut icon" href="${root}/favicon.ico" type="image/x-icon"/>
<link rel="stylesheet" type="text/css" href="${root}/css/commonStyle.css"/>
<link rel="stylesheet" type="text/css" href="${root}/css/menuStyle.css"/>
<link rel="stylesheet" type="text/css" href="${root}/css/loginModuleStyle.css"/>
<link rel="stylesheet" type="text/css" href="${root}/css/boardStyle.css"/>
<script type="text/javascript" src="${root}/javascript/util.js"></script>
<script type="text/javascript" src="${root}/javascript/md5.js"></script>
<script type="text/javascript" src="${root}/javascript/header.js"></script>
<script type="text/javascript" src="${root}/javascript/menuScript.js"></script>
<script type="text/javascript" src="${root}/javascript/login.js"></script>  
<script type="text/javascript">		
var root = "${root}";
var _userId = "${userId}";
var role = "${role}";

window.onload = init;

function init()
{
	initHeader();
	initMenu();
	initLogin();
}
</script>

<title>글 수정하기</title>
</head>
<body> 
	<div class="bodyWrapper"> 
		<%@ include file="/reuseModule/headerModule.jsp"%>
	 
		<div class="container">
			<%@ include file="/reuseModule/menuModule.jsp"%>
			
			<div class="left">
				<%@ include file="/reuseModule/loginModule.jsp"%>
				<div id="mainLeftBottom">
				</div>
			</div>
		
			<div class="center centerAlign">				
				<form method = "post" action="${root}/board/articleModifyComplete.do">
					<input type="hidden" name="boardName" value="${param.boardName}"/>
					<input type="hidden" name="articleIndex" value="${param.articleIndex}"/>
					<input type="hidden" name="currPage" value="${param.currPage}"/>
						
					<table class="articleWriteModifyTable">
			    		<tr>
							<td class="articleTitleLabel">제목</td>
			    			<td class="articleTitleContent">
			    				<input class="board" type = "text" name = "title" value="${article.title}">
			    			</td>
			    		</tr>				
    					<tr>
			    			<td class="articleTextContent" colspan="2">			    			    			
			    				<textarea class="board" name = "content">${article.text}</textarea>
			    			</td>			    			
			    		</tr>
						<tr>
							<td colspan = "2">
								<input class="articleModifyWrapper" type="image" src="${root}/image/board/articleModify.png" />
								<a class="articleListWrapper" href="${root}/board/articleList.do?boardName=${param.boardName}&currPage=${param.currPage}">
									<img src="${root}/image/board/toFirstPage.png"/>
								</a>
							</td>
			    		</tr>    					
    				</table>	
				</form>
			</div>
		
			<div class="right"></div>
		</div> 
	
		<%@ include file="/reuseModule/footerModule.jsp"%>
	</div>
</body>
</html>
