/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverStaticMethods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:24:23 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/01 18:40:38 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <algorithm>
#include <stdexcept>
#include "Client.hpp"

const Client &Server::getClientByNickname(const string &nickname) const
{
	auto it = std::find_if(_clients.begin(), _clients.end(),
		[&nickname](const std::pair<const int, Client *> &client)
		{
			return (*client.second == nickname);
		});
	if (it != _clients.end())
		return (*it->second);
	throw std::invalid_argument("Client not found");
}