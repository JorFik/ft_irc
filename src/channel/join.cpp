/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:04:23 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/13 18:06:57 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "Utils.hpp"
#include "numericReplies.hpp"
#include <stdexcept>

string	Channel::_getMembersList() const noexcept
{
	string membersList;
	size_t i = 0;

	for (const auto member : _members)
	{
		if (_operators.find(member) != _operators.end())
			membersList += "@";
		membersList += member->getNickname();
		if (i++ < _members.size() - 1)
			membersList += " ";
	}
	return (membersList);
}

void	Channel::_sendChannelInfo(Client &client)
{
	if (_topic.empty())
		client.addToSendBuffer(RPL_NOTOPIC(client.getNickname(), _name));
	else
		client.addToSendBuffer(RPL_TOPIC(client.getNickname(), _name, _topic));
	client.addToSendBuffer(RPL_NAMREPLY(client.getNickname(), _name, _getMembersList()));
	client.addToSendBuffer(RPL_ENDOFNAMES(client.getNickname(), _name));
	_broadcastMsg(":" + client.getNickname() + " JOIN " + _name + "\r\n", nullptr);
}

void	Channel::join(Client &client, const string &password)
{
	if (_mode.test(static_cast<size_t>(Mode::InviteOnly)) &&
		_invited.find(&client) == _invited.end())
		throw std::invalid_argument(ERR_INVITEONLYCHAN(client.getNickname(), _name));
	if (_mode.test(static_cast<size_t>(Mode::PasswordProtected)) &&
		_password != password)
		throw std::invalid_argument(ERR_BADCHANNELKEY(client.getNickname(), _name));
	if (_mode.test(static_cast<size_t>(Mode::UserLimit)) &&
		_members.size() >= _userLimit)
		throw std::invalid_argument(ERR_CHANNELISFULL(client.getNickname(), _name));
	_members.insert(&client);
	_sendChannelInfo(client);
}
