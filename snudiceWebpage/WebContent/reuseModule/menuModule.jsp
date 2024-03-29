<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>

<!-- menu -->
<div id="menu">
	<div id="logoWrapper">
		<img id="logo" src="${root}/image/mainpage/logo.png"/>
	</div>
	<div id="menuWrapper">
		<div id="menu1">
			소식
			<div id="menu1DropDown" class="menuDropDownWrapper invisible">
				<div id="notice">								
					<a href="${root}/board/articleList.do?boardName=${noticeBoardName}&currPage=0">${noticeBoardAliasName}</a>								
				</div>
				<div id="update">								
					<a href="${root}/board/articleList.do?boardName=${updateBoardName}&currPage=0">${updateBoardAliasName}</a>							
				</div>							
			</div>
		</div>
		<div id="menu2">
			게임소개
			<div id="menu2DropDown" class="menuDropDownWrapper invisible">
				<div id="synopsis">
					<a href="${root}/misc/gameSystem.do">게임시스템</a>
				</div>
				<div id="playGuide">
					<a href="${root}/misc/manual.do?page=1">게임방법</a>
				</div>
				<div id="character">
					<a href="${root}/misc/charIntro.do">캐릭터</a>							
				</div>
				<div id="card">
					<a href="${root}/misc/card.do">카드</a>
				</div>							
			</div>
		</div>
		<div id="menu3">
			PrjN
			<div id="menu3DropDown" class="menuDropDownWrapper invisible">
				<div id="teamIntro">
					<a href="${root}/misc/teamIntro.do">팀소개</a>
				</div>													
			</div>
		</div>
		<div id="menu4">
			게시판
			<div id="menu4DropDown" class="menuDropDownWrapper invisible">
				<div id="freeBoard">
					<a href="${root}/board/articleList.do?boardName=${freeBoardName}&currPage=0">${freeBoardAliasName}</a>
				</div>																		
			</div>
		</div>
		<div id="menu5">
			자료실
			<div id="menu5DropDown" class="menuDropDownWrapper invisible">
				<div id="gameDownload">	
					<a href="${root}/setupFile/SNUDiCE_setup.exe">게임 다운로드</a>										
				</div>
				<div id="wallPaper">월페이퍼</div>	
				<div id="screenShot">스크린샷</div>							
			</div>
		</div>
	</div>				
</div>
<div class="containerTop"></div>			