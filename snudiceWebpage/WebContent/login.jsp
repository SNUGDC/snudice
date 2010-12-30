<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix ="c" uri="http://java.sun.com/jsp/jstl/core" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<link rel="stylesheet" type="text/css" href="${root}/css/loginPageStyle.css"/>
<link rel="stylesheet" type="text/css" href="${root}/css/boardStyle.css"/>
<title>로그인 페이지</title>
</head>
<body>
	<div class="wrapper">
		<div class="header" id="loginHeader">
			<img src="${root}/image/mainpage/title.png"/>
		</div>
		<div class="container" id="loginContainer">
		<center>
			<div class="left">
				<div class="leftTop">
					<br/><br/>
					<form method="POST" action="login.do">
						<div class="idpwWrap">
							<div class="idpw">
								<input name="userId" type="text"><br/>
								<input name="password" type="password">
							</div>
							<input id="login" type="image" src="${root}/image/mainpage/login.png">
						</div>
					</form>	
					<div class="loginHelp">
						<a href = "${root}/join.jsp">회원가입</a>&nbsp;&nbsp;&nbsp;&nbsp;<a href = "${root}/notimplemented.jsp"> ID/PW찾기</a><br/>
					</div>
				</div>
				<div class="leftBottom">
					<br/>
					&nbsp;&nbsp;&nbsp;&nbsp;
					<img src="${root}/image/mainpage/start.png">
					&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
					<a href="${root}/clientFile/update/6001/a.txt"><img src="${root}/image/mainpage/download.png"></a>
				</div>
			</div>
			<div class="center">
				
				<div class="wrapper">
	<div class="header">
		<h1>게시판</h1>
	</div>
	<div class="container">
		<div class="left"></div>
		<div class="center">
			<table class="boardList">	
				<tr>
					<td class="articleNumber">번호</td>
					<td class="articleTitle">제목</td>
					<td class="articleWriter">글쓴이</td>
					<td class="articleDateTime">날짜</td>
					<td class="articleReadCount">조회수</td>
				</tr>
				
				<c:forEach var="article" items="${articleList}" varStatus="status">	
				<tr>
					<td>${article.articleIndex}</td>
					<td>
						<a href="${root}/articleView.do?boardName=${param.boardName}&articleIndex=${article.articleIndex}&currPage=${param.currPage}">
							${article.title} 
							<c:if test="${replyCountList[status.index]>0}">
								<span id="replyCountInTitle"> (${replyCountList[status.index]})</span>
							</c:if>
						</a>
					</td>
					<td>${article.userId}</td>
					<td>${article.dateTime}</td> 
					<td>${article.readCount}</td>		
				</tr>
				</c:forEach>
			</table>
			<table class="boardList">
				<tr>
				<td class="pageNumber"> <a href="${root}/articleList.do?boardName=${param.boardName}&currPage=${param.currPage}&goPrev=true">prev</a> </td>
				<c:forEach var="pageNumber" items="${pageList}">
					<td class="pageNumber">
					
					<c:choose>
						<c:when test="${pageNumber==param.currPage}">
							<a href="${root}/articleList.do?boardName=${param.boardName}&currPage=${pageNumber}"> (${param.currPage})</a>
						</c:when>
					<c:otherwise>
					<a href="${root}/articleList.do?boardName=${param.boardName}&currPage=${pageNumber}"> ${pageNumber}</a>
					</c:otherwise>
					</c:choose>
					</td>
				</c:forEach>
				<td class="pageNumber"> <a href="${root}/articleList.do?boardName=${param.boardName}&currPage=${param.currPage}&goNext=true">next</a> </td>
				</tr>
			</table>					
		</div>
		<div class="right"></div>
	</div>
	<div class="footer">
		현재 페이지 : ${param.currPage}  		
		<br/>
		<a href="${root}/articleList.do?boardName=${param.boardName}&currPage=0">처음목록</a>
		<a href="${root}/main.do">메인으로</a>	
		<a id="boardWrite" href="${root}/articleWriteForm.do?boardName=${param.boardName}&currPage=${param.currPage}">글쓰기</a>
		<a href="${root}/logout.do">로그아웃</a>
	</div>
	</div>
				
				
			</div>
			<div class="right">&nbsp;</div>	
			</center>
		</div>
		<div class="footer" id="loginFooter">		
		</div>			
	</div>
</body>
</html>